#pragma once

class CAutorun
{
public:
	CAutorun(void);
	~CAutorun(void);

	// добавить себя в автозагрузку
	void AddReg(void);
	// убрать себя из автозагрузки
	void RemoveReg(void);
	// включена автозагрузка или нет?
	bool IsAutorunReg(void);

	void Add(void);
	void Remove(void);
	bool IsAutorun(void);

private:
	char* m_RegRunSubKey;
	char* m_ExeFileName;
	char m_ExePath[MAX_PATH];
};

