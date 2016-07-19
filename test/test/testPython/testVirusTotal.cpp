#include <Python.h>

int scanVirustotal(char* filename)
{
	int result = 0;
	PyObject* mydef = PyImport_ImportModule("scripts.virustotal");		// ��� import

	if(mydef) {
		PyObject* scan_virustotal = PyObject_GetAttrString(mydef, "scan_virustotal");	// ������Ʈ ��

		if(scan_virustotal) {
			PyObject *scan = PyObject_CallFunction(scan_virustotal, "s", filename);	// ���� ���ڸ� ������ ���

			if (scan) {
				result = PyInt_Check(scan);
				Py_XDECREF(scan);		// ������ ������Ʈ�� ����
			}

			Py_XDECREF(scan_virustotal);
		}
		else {
			printf("PyObject_GetAttrString error!!\n");
		}
		Py_XDECREF(mydef);
	}
	else{
		printf("PyImport_ImportModule error!!\n");
	}

	return result;
}

int main(int argc, char** argv)//(int argc, char** argv)
{
	int result = 0;

	// ���� Ž��
	/*if(argc <= 1)
	{
		printf("usage python use_virustotal.py [filename]\n");
		return 0;
	}*/

	int testNum = 2;
	char** testPath;
	testPath = new char* [2];
	strncpy(testPath[0], argv[0], sizeof(argv[0]));
	strncpy(testPath[1], "virustotal.py", sizeof("virustotal.py"));

	Py_SetProgramName(argv[0]);
	Py_Initialize();

	if (Py_IsInitialized()) {
		PySys_SetArgv(testNum, testPath);	// ���̽㿡 ���� ����
		
		char* testname = "test.txt";
		result = scanVirustotal(testname);		// ���̽� �Լ� ����
		printf("*** result : %d\n", result);
		Py_Finalize();
	}
	getchar();
	return 0;
}
