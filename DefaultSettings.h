#pragma once

class CDefaulSettings
{
public:
	CDefaulSettings(void);
	~CDefaulSettings(void);

	bool IsDefaultSettings();
	bool WriteSettings(int Hour, int Minute, int Seconds, int OperationMode, int ToDo, int ToDoAfter);
	bool ReadSettings(int& outHour, int& outMinute, int& outSeconds, int& outOperationMode, int& outToDo, int& outToDoAfter);
	void ResetSettings();

private:
	CString m_RegSubKey;
	//названия ключей в реестре
	const CString csDefSettingsHour				;
	const CString csDefSettingsMinute			;
	const CString csDefSettingsSeconds			;
	const CString csDefSettingsOperationMode	;
	const CString csDefSettingsToDo				;
	const CString csDefSettingsToDoAfter		;
	const CString csDefSettingsRegSubKey		;
};

