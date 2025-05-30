#pragma once


#define DECLARE_SINGLE(classname)			\
private:									\
	classname() { }							\
public:										\
	static classname& GetInstance()			\
	{										\
		static classname s_instance;		\
		return s_instance;					\
	}


#define GET_SINGLE(classname)	classname::GetInstance()

#define INPUT			GET_SINGLE(InputManager)
#define RENDER			GET_SINGLE(RenderManager)
#define SHADER			GET_SINGLE(ShaderManager)