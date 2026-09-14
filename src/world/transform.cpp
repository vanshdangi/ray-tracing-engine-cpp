#include <world/transform.hpp>
#include <maths/matrix4.hpp>

Transform::Transform()
    : position(0.0f, 0.0f, 0.0f),
      rotation(0.0f, 0.0f, 0.0f),
      scale(1.0f, 1.0f, 1.0f)
{}

Transform::Transform(Point3 position_, Vec3 rotation_, Vec3 scale_)
    : position(position_),
      rotation(rotation_),
      scale(scale_)
{}

Matrix4 Transform::getMatrix() const {
    Matrix4 matrix;

    matrix *= Matrix4::makeTranslation(position);
    matrix *= Matrix4::makeRotationZ(rotation.z);
    matrix *= Matrix4::makeRotationY(rotation.y);
    matrix *= Matrix4::makeRotationX(rotation.x);
    matrix *= Matrix4::makeScale(scale);

    return matrix;
}

Matrix4 Transform::getInverseMatrix() const {
    Matrix4 matrix;

    matrix *= Matrix4::makeScale(
        Vec3(
            1.0f / scale.x,
            1.0f / scale.y,
            1.0f / scale.z
        )
    );

    matrix *= Matrix4::makeRotationX(-rotation.x);
    matrix *= Matrix4::makeRotationY(-rotation.y);
    matrix *= Matrix4::makeRotationZ(-rotation.z);

    matrix *= Matrix4::makeTranslation(
        Point3(-position.x, -position.y, -position.z)
    );

    return matrix;
}

Ray Transform::toLocal(const Ray& worldRay) const {
    Matrix4 inverseMatrix = getInverseMatrix();

    return {
        inverseMatrix * worldRay.origin,
        inverseMatrix * worldRay.direction
    };
}

Point3 Transform::toWorld(const Point3& localPoint) const {
    return getMatrix() * localPoint;
}

Vec3 Transform::transformNormal(const Vec3& localNormal) const {
    Matrix4 normalMatrix;

    normalMatrix *= Matrix4::makeRotationZ(rotation.z);
    normalMatrix *= Matrix4::makeRotationY(rotation.y);
    normalMatrix *= Matrix4::makeRotationX(rotation.x);

    normalMatrix *= Matrix4::makeScale(
        Vec3(
            1.0f / scale.x,
            1.0f / scale.y,
            1.0f / scale.z
        )
    );

    return (normalMatrix * localNormal).normalized();
}