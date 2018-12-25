#include "Shader.h"

void BaseVertexShader::Main() {
    outClipPosition = projection * view * model * Vec4f{ inPosition, 1.0f };
    outWorldPosition = model * Vec4f{ inPosition, 1.0f };
    outWorldNormal = model * Vec4f{ inNormal, 0.0f };
    outUV = inUV;

    outDepthViewInverse = 1 / outClipPosition.w;
    outUV.x *= outDepthViewInverse;
    outUV.y *= outDepthViewInverse;
}

void PhongFragmentShader::Main() {
    assert(material && lights);

    inUV.x /= inDepthViewInverse;
    inUV.y /= inDepthViewInverse;

    Vec3f materialAmbient = { 1, 1, 1 };
    Vec3f materialAlbedo = { 1, 1, 1 };
    Vec3f materialSpecular = { 1, 1, 1 };
    Vec3f materialEmission = { 0, 0, 0 };
    float_t materialShininess = 10.0f;

    if (material) {
        float_t sample[4];

        // Ambient
        if (material->ambientMap) {
            material->ambientMap->Sample(inUV.u, inUV.v, sample);
            materialAmbient.Set(sample);
        }
        else {
            materialAmbient = material->ambient;
        }

        // Albedo
        if (material->albedoMap) {
            material->albedoMap->Sample(inUV.u, inUV.v, sample);
            materialAlbedo.Set(sample);
        }
        else {
            materialAlbedo = material->albedo;
        }

        // Specular
        if (material->specularMap) {
            material->specularMap->Sample(inUV.u, inUV.v, sample);
            materialSpecular.Set(sample);
        }
        else {
            materialSpecular = material->specular;
        }

        // Emission
        if (material->emissionMap) {
            material->emissionMap->Sample(inUV.u, inUV.v, sample);
            materialEmission.Set(sample);
        }
        else {
            materialEmission = material->emission;
        }

        // Shininess
        materialShininess = material->shininess;
    }

    inWorldNormal.Normalize();

    Vec3f result{ 0.0f };

    for (const auto &light : *lights) {
        Vec3f lightDirection;
        float_t attenuation = 1.0f;
        if (light.lightType == LightType::Directional) {
            lightDirection = Normalize(-1.0f * light.direction);
        }
        else {
            lightDirection = light.position - inWorldPosition;
            float_t lightDistance = lightDirection.Length();
            lightDirection = Normalize(lightDirection);
            attenuation = 1.0f / (light.constant + light.linear * lightDistance + light.quadratic * (lightDistance * lightDistance));
        }

        // Ambient
        Vec3f ambient = attenuation * materialAmbient * Vec3f{ light.ambient };

        // Albedo
        Vec3f albedo = attenuation * std::max(Dot(inWorldNormal, lightDirection), 0.0f) * materialAlbedo * Vec3f { light.diffuse };

        // Specular
        bool blinn = true;
        float_t spec = 0.0f;
        Vec3f viewDirection = Normalize(viewPosition - inWorldPosition);
        if (blinn) {
            spec = std::powf(std::max(Dot(inWorldNormal, Normalize(lightDirection + viewDirection)), 0.0f), 3.0f * materialShininess);
        }
        else {
            Vec3f reflectDirection = Reflect(-1.0f * lightDirection, inWorldNormal);
            spec = std::powf(std::max(Dot(viewDirection, reflectDirection), 0.0f), materialShininess);
        }

        Vec3f specular = attenuation * spec * materialSpecular * Vec3f{ light.specular };

        result += ambient + albedo + specular;
    }

    result += materialEmission;

    outFragColor = Vec4f{ result, 1.0f };
}
