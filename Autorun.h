#pragma once

class CAutorun
{
public:
	CAutorun(void);
	~CAutorun(void);

	// �������� ���� � ������������
	void AddReg(void);
	// ������ ���� �� ������������
	void RemoveReg(void);
	// �������� ������������ ��� ���?
	bool IsAutorunReg(void);

	void Add(void);
	void Remove(void);
	bool IsAutorun(void);

private:
	char* m_RegRunSubKey;
	char* m_ExeFileName;
	char m_ExePath[MAX_PATH];
};

