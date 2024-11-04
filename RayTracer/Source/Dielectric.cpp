#include "Dielectric.h"
#include "Random.h"

bool Dielectric::Scatter(const ray_t& ray, const raycastHit_t& raycastHit, color3_t& attenuation, ray_t& scatter)
{
    glm::vec3 outNormal;
    float refractiveIndexModified; 
    float cosine;

    // Check if hitting from the outside
    if (Math::Dot(ray.direction, raycastHit.normal) < 0)
    {
        outNormal = raycastHit.normal;
        refractiveIndexModified = 1.0f / m_refractiveIndex;
        cosine = -Math::Dot(ray.direction, raycastHit.normal) / glm::length(ray.direction);
    }
    else // hitting from the inside
    {
        outNormal = -raycastHit.normal;
        refractiveIndexModified = m_refractiveIndex;
        cosine = m_refractiveIndex * Math::Dot(ray.direction, raycastHit.normal) / glm::length(ray.direction);
    }

    glm::vec3 refracted;
    float reflectProbability = 1.0f;

    if (Math::Refract(ray.direction, outNormal, refractiveIndexModified, refracted))
    {
        reflectProbability = Math::Schlick(cosine, m_refractiveIndex);
    }

    glm::vec3 reflected = Math::Reflect(ray.direction, raycastHit.normal);

    scatter = (randomf() < reflectProbability) ? ray_t{ raycastHit.point, reflected } : ray_t{ raycastHit.point, refracted };

    attenuation = m_albedo;

    return true;
}
