#pragma once

namespace Vector3 {

	inline XMFLOAT3 XMVectorToFloat3(XMVECTOR& xmVector) {
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, xmVector);
		return xmf3Result;
	}

	inline XMFLOAT3 ScalarProduct(const XMFLOAT3 xmf3Vector, float fScalar, BOOL bNormalize = TRUE) {
		XMFLOAT3 xmf3Result;
		if (bNormalize) {
			return XMVectorToFloat3(XMVector3Normalize(XMVectorScale(XMLoadFloat3(&xmf3Vector), fScalar)));
		}
		else {
			return XMVectorToFloat3(XMVectorScale(XMLoadFloat3(&xmf3Vector), fScalar));
		}
	}

	inline XMFLOAT3 Add(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2) {
		return XMVectorToFloat3(XMVectorAdd(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2)));
	}

	inline XMFLOAT3 Add(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2, float fScalar) {
		return XMVectorToFloat3(XMVectorAdd(XMLoadFloat3(&xmf3Vector1), XMVectorScale(XMLoadFloat3(&xmf3Vector2), fScalar)));
	}

	inline XMFLOAT3 Subtract(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2) {
		return XMVectorToFloat3(XMVectorSubtract(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2)));
	}

	inline float DotProduct(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2) {
		return XMVectorGetX(XMVector3Dot(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2)));
	}

	inline XMFLOAT3 CrossProduct(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2, BOOL bNormalize = TRUE) {
		if (bNormalize) {
			return XMVectorToFloat3(XMVector3Normalize(XMVector3Cross(XMVector3Normalize(XMLoadFloat3(&xmf3Vector1)), XMVector3Normalize(XMLoadFloat3(&xmf3Vector2)))));
		}
		else {
			return XMVectorToFloat3(XMVector3Cross(XMVector3Normalize(XMLoadFloat3(&xmf3Vector1)), XMVector3Normalize(XMLoadFloat3(&xmf3Vector2))));
		}
	}

	inline XMFLOAT3 Normalize(const XMFLOAT3& xmf3Vector) {
		return XMVectorToFloat3(XMVector3Normalize(XMLoadFloat3(&xmf3Vector)));
	}

	inline float Length(const XMFLOAT3& xmf3Vector) {
		return XMVectorGetX(XMVector3Length(XMLoadFloat3(&xmf3Vector)));
	}

	inline float Distance(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2) {
		return XMVectorGetX(XMVector3Length(XMVectorSubtract(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2))));
	}

	inline float Angle(XMVECTOR& xmvVector1, XMVECTOR& xmvVector2) {
		assert(XMVectorGetX(XMVector3Length(xmvVector1)) == 1.0f);
		assert(XMVectorGetX(XMVector3Length(xmvVector2)) == 1.0f);

		return XMVectorGetX(XMVectorACos(XMVector3AngleBetweenNormals(xmvVector1, xmvVector2)));
	}

	inline float Angle(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2) {
		return Angle(XMVector3Normalize(XMLoadFloat3(&xmf3Vector1)), XMVector3Normalize(XMLoadFloat3(&xmf3Vector2)));
	}

	inline XMFLOAT3 TransformNormal(const XMFLOAT3& xmf3Vector, const XMMATRIX& xmf4x4Transform) {
		return XMVectorToFloat3(XMVector3TransformNormal(XMLoadFloat3(&xmf3Vector), xmf4x4Transform));
	}

	inline XMFLOAT3 TransformNormal(const XMFLOAT3& xmf3Vector, const XMFLOAT4X4& xmmtx4x4Matrix) {
		return TransformNormal(xmf3Vector, XMLoadFloat4x4(&xmmtx4x4Matrix));
	}

	inline XMFLOAT3 TransformCoord(const XMFLOAT3& xmf3Vector, const XMMATRIX& xmxm4x4Transform) {
		return XMVectorToFloat3(XMVector3TransformCoord(XMLoadFloat3(&xmf3Vector), xmxm4x4Transform));
	}

	inline XMFLOAT3 TransformCoord(const XMFLOAT3& xmf3Vector, const XMFLOAT4X4& xmmtx4x4Matrix) {
		return TransformCoord(xmf3Vector, XMLoadFloat4x4(&xmmtx4x4Matrix));
	}

}

namespace Vector4 {

	inline XMFLOAT4 XMVectorToFloat4(XMVECTOR& xmVector) {
		XMFLOAT4 xmf3Result;
		XMStoreFloat4(&xmf3Result, xmVector);
		return xmf3Result;
	}

	inline XMFLOAT4 Add(const XMFLOAT4& xmf4Vector1, const XMFLOAT4& xmf4Vector2) {
		XMFLOAT4 xmf4Result;
		XMStoreFloat4(&xmf4Result, XMVectorAdd(XMLoadFloat4(&xmf4Vector1), XMLoadFloat4(&xmf4Vector2)));
		return xmf4Result;
	}
}

namespace Matrix4x4 {

	inline XMFLOAT4X4 XMMatrixToFloat4x4(const XMMATRIX& m) {
		XMFLOAT4X4 xmf4x4Matrix;
		XMStoreFloat4x4(&xmf4x4Matrix, m);
		return xmf4x4Matrix;
	}

	inline XMFLOAT4X4 Identity() {
		return XMMatrixToFloat4x4(XMMatrixIdentity());
	}

	inline XMFLOAT4X4 Translate(float x, float y, float z) {
		return XMMatrixToFloat4x4(XMMatrixTranslation(x, y, z));
	}
	
	inline XMFLOAT4X4 Translate(const XMVECTOR& xmvVector) {
		return XMMatrixToFloat4x4(XMMatrixTranslationFromVector(xmvVector));
	}
	
	inline XMFLOAT4X4 Translate(const XMFLOAT3& xmf3Vector) {
		return Translate(XMLoadFloat3(&xmf3Vector));
	}

	inline XMFLOAT4X4 Multiply(const XMMATRIX& xmmtxMatrix1, const XMMATRIX& xmmtxMatrix2) {
		return XMMatrixToFloat4x4(XMMatrixMultiply(xmmtxMatrix1, xmmtxMatrix2));
	}

	inline XMFLOAT4X4 Multiply(const XMFLOAT4X4& xmf4x4Matrix1, const XMMATRIX& xmmtxMatrix2) {
		return Multiply(XMLoadFloat4x4(&xmf4x4Matrix1), xmmtxMatrix2);
	}
	
	inline XMFLOAT4X4 Multiply(const XMMATRIX& xmmtxMatrix1, const XMFLOAT4X4& xmf4x4Matrix2) {
		return Multiply(xmmtxMatrix1, XMLoadFloat4x4(&xmf4x4Matrix2));
	}
	
	inline XMFLOAT4X4 Multiply(const XMFLOAT4X4& xmf4x4Matrix1, const XMFLOAT4X4& xmf4x4Matrix2) {
		return Multiply(XMLoadFloat4x4(&xmf4x4Matrix1), XMLoadFloat4x4(&xmf4x4Matrix2));
	}

	inline XMFLOAT4X4 RotationYawPitchRoll(float fPitch, float fYaw, float fRoll) {
		return XMMatrixToFloat4x4(XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch), XMConvertToRadians(fYaw), XMConvertToRadians(fRoll)));
	}

	inline XMFLOAT4X4 RotationAxis(const XMFLOAT3& xmf3Axis, float fAngle) {
		return XMMatrixToFloat4x4(XMMatrixRotationAxis(XMLoadFloat3(&xmf3Axis), XMConvertToRadians(fAngle)));
	}

	inline XMFLOAT4X4 Inverse(const XMFLOAT4X4& xmf4x4Matrix) {
		return XMMatrixToFloat4x4(XMMatrixInverse(NULL, XMLoadFloat4x4(&xmf4x4Matrix)));
	}

	inline XMFLOAT4X4 Transpose(const XMFLOAT4X4& xmf4x4Matrix) {
		return XMMatrixToFloat4x4(XMMatrixTranspose(XMLoadFloat4x4(&xmf4x4Matrix)));
	}

	inline XMFLOAT4X4 PerspectiveFovLH(float fFOVAngleY, float fAspectRatio, float fNearZ, float fFarZ) {
		return XMMatrixToFloat4x4(XMMatrixPerspectiveFovLH(fFOVAngleY, fAspectRatio, fNearZ, fFarZ));
	}

	inline XMFLOAT4X4 LookAtLH(const XMFLOAT3& xmf3EyePosition, const XMFLOAT3& xmf3LookAtPosition, const XMFLOAT3& xmf3UpDirection) {
		return XMMatrixToFloat4x4(XMMatrixLookAtLH(XMLoadFloat3(&xmf3EyePosition), XMLoadFloat3(&xmf3LookAtPosition), XMLoadFloat3(&xmf3UpDirection)));
	}

	inline XMFLOAT4X4 LookToLH(const XMFLOAT3& xmf3EyePosition, const XMFLOAT3& xmf3LookTo, const XMFLOAT3 xmf3UpDirection) {
		return XMMatrixToFloat4x4(XMMatrixLookToLH(XMLoadFloat3(&xmf3EyePosition), XMLoadFloat3(&xmf3LookTo), XMLoadFloat3(&xmf3UpDirection)));
	}

}

namespace Triangle {

	inline BOOL Intersect(const XMFLOAT3& xmf3RayPosition, const XMFLOAT3& xmf3RayDirection, const XMFLOAT3& v0, const XMFLOAT3& v1, const XMFLOAT3& v2, float& fHitDistance) {
		return TriangleTests::Intersects(XMLoadFloat3(&xmf3RayPosition), XMLoadFloat3(&xmf3RayDirection), XMLoadFloat3(&v0), XMLoadFloat3(&v1), XMLoadFloat3(&v2), fHitDistance);
	}

}

namespace Plane {
	
	inline XMFLOAT4 Normalize(const XMFLOAT4& xmf4Plane) {
		return Vector4::XMVectorToFloat4(XMPlaneNormalize(XMLoadFloat4(&xmf4Plane)));
	}

}
//
//namespace Quaternion {
//
//	inline void ExtractEulerAnglesFromQuaternion(OUT XMFLOAT3& xmf3Euler, IN const XMFLOAT4& xmf4Quaternion, BOOL bToDegree = TRUE) {
//		XMMATRIX xmmtxRotationFromQuat = XMMatrixRotationQuaternion(XMLoadFloat4(&xmf4Quaternion));
//		XMFLOAT4X4 xmf4x4RotationFromQuat;
//		XMStoreFloat4x4(&xmf4x4RotationFromQuat, xmmtxRotationFromQuat);
//
//		float fPitch, fYaw, fRoll;
//		
//		fPitch = std::asinf(-xmf4x4RotationFromQuat._32);
//		
//		if (std::cosf(fPitch) > 1e-6f) {
//			fYaw = atan2f(xmf4x4RotationFromQuat._31, xmf4x4RotationFromQuat._33);
//			fRoll = atan2f(xmf4x4RotationFromQuat._12, xmf4x4RotationFromQuat._22);
//		}
//		else {
//			fYaw = atan2f(-xmf4x4RotationFromQuat._21, xmf4x4RotationFromQuat._11);
//			fRoll = 0.f;
//		}
//
//		if (bToDegree){
//			fPitch = XMConvertToDegrees(fPitch);
//			fYaw = XMConvertToDegrees(fYaw);
//			fRoll = XMConvertToDegrees(fRoll);
//		}
//
//		xmf3Euler = XMFLOAT3{ fPitch, fYaw, fRoll };
//
//	}
//
//}