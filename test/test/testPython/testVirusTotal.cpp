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

int main(int argc, char** argv)
{
	int result = 0;

	// ���� Ž��
	if(argc <= 1)
	{
		printf("usage python use_virustotal.py [filename]\n");
		return 0;
	}

	Py_SetProgramName(argv[0]);
	Py_Initialize();

	if (Py_IsInitialized()) {
		PySys_SetArgv(argc, argv);	// ���̽㿡 ���� ����
		
		char* testname = "test.txt";
		result = scanVirustotal(testname);		// ���̽� �Լ� ����
		printf("*** result : %d\n", result);
		Py_Finalize();
	}
	return 0;
}
