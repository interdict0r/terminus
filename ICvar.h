#pragma once
template <typename t>
static t GetVFunc(void* class_pointer, size_t index) {
	return (*(t**)class_pointer)[index];
}
class ConVar
{
public:
	int GetInt(void) const {
		return pParent->nValue;
	}
	const char* GetString(void) const {
		return pParent->pszDefaultValue;
	}
	float GetFloat() {
		typedef float(__thiscall* Fn)(void*);
		return GetVFunc< Fn >(this, 12)(this);
	}

	char pad_0x0000[0x4]; // 0x0000
	ConVar* pNext; // 0x0004 
	__int32 bRegistered; // 0x0008 
	char* pszName; // 0x000C 
	char* pszHelpString; // 0x0010 
	__int32 nFlags; // 0x0014 
	char pad_0x0018[0x4]; // 0x0018
	ConVar* pParent; // 0x001C 
	char* pszDefaultValue; // 0x0020 
	char* strString; // 0x0024 
	__int32 StringLength; // 0x0028 
	float fValue; // 0x002C 
	__int32 nValue; // 0x0030 
	__int32 bHasMin; // 0x0034 
	float fMinVal; // 0x0038 
	__int32 bHasMax; // 0x003C 
	float fMaxVal; // 0x0040 
	void *fnChangeCallback; // 0x0044 


};// Size=0x0048


class IAppSystem {
public:
	virtual ~IAppSystem() {}

	virtual void func0() = 0;
	virtual void func1() = 0;
	virtual void func2() = 0;
	virtual void func3() = 0;
	virtual void func4() = 0;
	virtual void func5() = 0;
	virtual void func6() = 0;
	virtual void func7() = 0;
	virtual void func8() = 0;
	virtual void func9() = 0;
};

struct CVarDLLIdentifier_t;

class icvar : public IAppSystem {
public:
	virtual ConVar			*FindVar(const char *var_name) = 0;
};

extern icvar* cvar;