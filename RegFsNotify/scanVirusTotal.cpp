#include "mon.h"

int scanVirusTotal(char* filename)
{
	int result = 0;
	PyObject* mydef = PyImport_ImportModule("scripts.virustotal");		// 모듈 import

	if(mydef) {
		PyObject* scan_virustotal = PyObject_GetAttrString(mydef, "scan_virustotal");	// 오브젝트 얻어냄

		if(scan_virustotal) {
			PyObject *scan = PyObject_CallFunction(scan_virustotal, "s", filename);	// 전달 인자를 가지고 출력

			if (scan) {
				result = PyInt_Check(scan);
				Py_XDECREF(scan);		// 생성된 오브젝트를 정리
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

//int main(int argc, char** argv)
//{
//	int result = 0;
//
//	// 에러 탐지
//	if(argc <= 1)
//	{
//		printf("usage python use_virustotal.py [filename]\n");
//		return 0;
//	}
//
//	Py_SetProgramName(argv[0]);
//	Py_Initialize();
//
//	if (Py_IsInitialized()) {
//		PySys_SetArgv(argc, argv);	// 파이썬에 인자 전달
//		
//		char* testname = "0c5b7a6592af5261e50f38250c6658b77be4badd904905c8255ee41c32dbc2d7";
//		result = scanVirustotal(testname);		// 파이썬 함수 실행
//		Py_Finalize();
//	}
//	return 0;
//}
