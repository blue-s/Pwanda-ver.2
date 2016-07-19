#include "mon.h"

 // [2]�� ���� �Լ� �߰�
BOOL GetProcessElevation(TOKEN_ELEVATION_TYPE *pElevationType, BOOL *pIsAdmin)
{
	HANDLE hToken = NULL;
    BOOL bResult = FALSE;
    DWORD dwSize = 0;
 
    // ���� ���μ����� ��ū�� ��´�.
    if ( !OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken) )
        return FALSE;
 
    // ���ѻ�� ���¿� ���� ������ ��´�.
    if ( GetTokenInformation(hToken, TokenElevationType, pElevationType, sizeof(TOKEN_ELEVATION_TYPE), &dwSize) )
    {
        BYTE adminSID[SECURITY_MAX_SID_SIZE];
        dwSize = sizeof(adminSID);
         
        // ������ �׷��� SID ���� �����Ѵ�.
        CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, &adminSID, &dwSize);
 
        if ( *pElevationType == TokenElevationTypeLimited )
        {
            HANDLE hUnfilteredToken = NULL;
             
            // ����� ��ū�� �ڵ��� ��´�.
            GetTokenInformation(hToken, TokenLinkedToken, (void *)&hUnfilteredToken, sizeof(HANDLE), &dwSize);
 
            // ������ ��ū�� �������� SID�� �����ϰ� �ִ��� ���θ� Ȯ���Ѵ�.
            if ( CheckTokenMembership(hUnfilteredToken, &adminSID, pIsAdmin) )
                bResult = TRUE;
             
            CloseHandle(hUnfilteredToken);
        }
        else
        {
            *pIsAdmin = IsUserAnAdmin();
            bResult = TRUE;
        }
    }
 
    CloseHandle(hToken);
    return bResult;
}

void checkState(){
	TOKEN_ELEVATION_TYPE t;
    BOOL bAdmin = FALSE;
    TCHAR szUser[0xFF] = {0};
    DWORD dwUser = _countof(szUser);
    GetUserName(szUser, &dwUser);
 
    _tsetlocale(LC_ALL, _T("korean"));
 
    if ( GetProcessElevation(&t, &bAdmin) )
    {
        if ( bAdmin )
            _tprintf(_T("%s is admin, "), szUser);
        else
            _tprintf(_T("%s is not admin, "), szUser);
 
        switch (t)
        {
        case TokenElevationTypeDefault:
            _tprintf(_T("�⺻ ������̰ų�, UAC ����� OFF �� ������.\n"));
            break;
        case TokenElevationTypeFull:
            _tprintf(_T("���ѻ�µǾ���\n"));	// �츮�� ���ϴ� �κ�
            break;
        case TokenElevationTypeLimited:
            _tprintf(_T("���͵� ��ū�� ���� ���ѵ� �������� ����Ǿ���.\n"));
            break;
        default:
            break;
        }
    }
}