#pragma once

static std::default_random_engine g_dre{ std::random_device{}() };

class RandomGenerator {
public:
	static int GenerateRandomIntInRange(int min, int max) {
		return std::uniform_int_distribution<int>{ min, max }(g_dre);
	}

	static float GenerateRandomFloatInRange(float min, float max) {
		return std::uniform_real_distribution<float>{min, max}(g_dre);
		 
	}

	static COLORREF GenerateRandomColor() {
		std::uniform_int_distribution<int> uid{ 0, 255 };
		return RGB(uid(g_dre), uid(g_dre), uid(g_dre));
	}

	static XMVECTOR GenerateRandomUnitVectorOnSphere() {
		XMVECTOR xmvOne = XMVectorReplicate(1.0f);
		XMVECTOR xmvZero = XMVectorZero();

		while (true) {
			XMVECTOR v = XMVectorSet(GenerateRandomFloatInRange(-1.f, 1.f), GenerateRandomFloatInRange(-1.f, 1.f), GenerateRandomFloatInRange(-1.f, 1.f), 0.0f);
			if (!XMVector3Greater(XMVector3LengthSq(v), xmvOne)) {
				return XMVector3Normalize(v);
			}
		}

	}
};