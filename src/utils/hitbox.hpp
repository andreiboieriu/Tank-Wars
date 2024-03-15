#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <vector>


struct HitBox {
    glm::vec3 center;
    float rotationY;
    float lengthOX;
    float lengthOZ;
    float lengthOY;

    int id = -1;

    // HitBox(glm::vec3 center,
    //        float rotationY,
    //        float lengthOX,
    //        float lengthOZ) :
    //        center{center},
    //        rotationY{rotationY},
    //        lengthOX{lengthOX},
    //        lengthOZ{lengthOZ} {}

    bool CheckCollision(HitBox hitBox) {
        // check height
        if (center.y > hitBox.center.y + hitBox.lengthOY / 2.f ||
            center.y < hitBox.center.y - hitBox.lengthOY / 2.f)
            return false;

        std::vector<glm::vec3> myPoints = GetPoints();
        std::vector<glm::vec3> otherPoints = hitBox.GetPoints();

        float myArea = GetRectangleArea(myPoints);
        float otherArea = GetRectangleArea(otherPoints);

        for (auto& point : myPoints) {
            float area = GetRectangleArea(otherPoints, point);
            if (otherArea <= area + 0.01f && otherArea >= area - 0.01f)
                return true;
        }

        for (auto& point : otherPoints) {
            float area = GetRectangleArea(myPoints, point);

            if (myArea <= area + 0.01f && myArea >= area - 0.01f)
                return true;
        }

        return false;
    }

    float GetRectangleArea(std::vector<glm::vec3> points) {
        return glm::distance(points[0], points[1]) * glm::distance(points[1], points[2]);
    }

    float GetRectangleArea(std::vector<glm::vec3> points, glm::vec3 p) {
        float area = 0.f;

        area += 0.5f * glm::abs(points[0].x * (points[1].z - p.z) +
                                points[1].x * (p.z - points[0].z) +
                                p.x * (points[0].z - points[1].z));

        area += 0.5f * glm::abs(points[1].x * (points[2].z - p.z) +
                                points[2].x * (p.z - points[1].z) +
                                p.x * (points[1].z - points[2].z));

        area += 0.5f * glm::abs(points[2].x * (points[3].z - p.z) +
                                points[3].x * (p.z - points[2].z) +
                                p.x * (points[2].z - points[3].z));

        area += 0.5f * glm::abs(points[0].x * (points[3].z - p.z) +
                                points[3].x * (p.z - points[0].z) +
                                p.x * (points[0].z - points[3].z));

        return area;
    }

    std::vector<glm::vec3> GetPoints() {
        std::vector<glm::vec3> points;

        glm::vec3 halfOX = glm::vec3(lengthOX / 2.f, 0.f, 0.f);
        glm::vec3 halfOZ = glm::vec3(0.f, 0.f, lengthOZ / 2.f);

        halfOX = glm::rotateY(halfOX, glm::radians(rotationY));
        halfOZ = glm::rotateY(halfOZ, glm::radians(rotationY));

        points.push_back(center + halfOX + halfOZ);
        points.push_back(center + halfOX - halfOZ);
        points.push_back(center - halfOX - halfOZ);
        points.push_back(center - halfOX + halfOZ);

        return points;
    }
};