//#include <Python.h>
//
//void PrintMyDef()
//{
//	// 1. 먼저 모듈을 import한다.
//	PyObject* mydef = PyImport_ImportModule("scripts.my_def");
//		/*
//		*** 소스 코드의 경로
//		C 실행 파일이 생성되는 곳에 “scripts”라는 디렉토리를 하나 만들고,
//		그곳에 my_def.py 파일을 저장한다.
//		*/
//
//	// 2. 모듈이 임포트 되어서 모듈을 참조하는 오브젝트를 얻으면,
//	if(mydef) {
//		// 각 함수를 참조하는 오브젝트를 얻어낸다.
//		PyObject* printHello = PyObject_GetAttrString(mydef, "PrintMyDef");
//
//		// 3. 함수의 오브젝트도 얻어지면, 
//		if(printHello) {
//			// 그 함수를 불러낸다.
//			// PrintMyDef() 함수는 None 오브젝트를 리턴한다.
//			// 이것은 C의 NULL 포인터와는 다르다.
//			// 확실히 메모리를 점유하고 있는 객체이다.
//			PyObject *r = PyObject_CallFunction(printHello, NULL);
//
//			if (r == Py_None) {
//				printf("None is returned.\n");
//				Py_XDECREF(r);
//			}
//
//			// 4. 함수가 불러지고 나면 생성된 오브젝트들을
//			// 각각 정리(Py_XDECREF)한다.
//			Py_XDECREF(printHello);
//		}
//		Py_XDECREF(mydef);
//	}
//	else{
//		printf("PrintMyDef error!!\n");
//	}
//}
//
//int Multiply(int x, int y)
//{
//	// 1. 먼저 모듈을 import 한다.
//	PyObject* mydef = PyImport_ImportModule("scripts.my_def");
//
//	int result;
//	// 2. 모듈이 임포트 되어서 모듈을 참조하는 오브젝트를 얻으면,
//	if(mydef) {
//		// 각 함수를 참조하는 오브젝트를 얻어낸다.
//		PyObject* multiply = PyObject_GetAttrString(mydef, "Multiply");
//		
//		// 3. 함수의 오브젝트도 얻어지면,
//		if(multiply) {
//			// 그 함수를 불러낸다.
//			// 인자 2개를 집어넣고 결과를 받아온다.
//			PyObject *r = PyObject_CallFunction(multiply, "ii", x, y);
//			if(r) {
//				result = (int)PyInt_AS_LONG(r);
//				Py_XDECREF(r);
//			}
//
//			// 4. 함수가 불러지고 나면 생성된 오브젝트들을
//			// 각각 정리(Py_XDECREF)한다.
//			Py_XDECREF(multiply);
//		}
//		Py_XDECREF(mydef);
//	}
//	return result;
//}
//
//void SysPath()
//{
//	printf("sys.path:");
//	PyRun_SimpleString("import sys\nprint sys.path\nprint\n");
//		/*
//		*** PyRun_SimpleString
//		파이썬 스크립트가 어떤 모듈을 가져오기(import) 위해서는
//		모듈의 경로를 미리 알고 있어야 한다.
//		이 경로는 보통 sys.path에 기록된다.
//		PySys_SetArgv() 함수를 지정해 준다면 현재 스크립트가 실행되는 경로를 기준으로
//		sys.path에 기록된다.
//		그러나 다른 경로에 있는 모듈을 파이썬 스크립트를 사용하고 싶다면
//		반드시 이 경로를 입력해 주어야 한다.
//
//		sys.path를 호출해서 경로가 제대로 나오는지 확인한다.
//
//		['C:\\Users\\kosta\\Desktop\\test\\test\\Release',
//		'C:\\Python27\\lib\\site-packages\\python_keystoneclient-3.2.0-py2.7.egg',
//		'C:\\Python27\\lib\\site-packages\\stevedore-1.16.0-py2.7.egg',
//		'C:\\Python27\\lib\\site-packages\\requests-2.10.0-py2.7.egg',
//		'C:\\Python27\\lib\\site-packages\\positional-1.1.1-py2.7.egg',
//		'C:\\Python27\\lib\\site-packages\\oslo.utils-3.16.0-py2.7.egg',
//		'C:\\Python27\\lib\\site-packages\\oslo.serialization-2.11.0-py2.7.egg',
//		'C:\\Python27\\lib\\site-packages\\oslo.i18n-3.8.0-py2.7.egg',
//		'C:\\Python27\\lib\\site-packages\\oslo.config-3.13.0-py2.7.egg',
//		'C:\\Python27\\lib\\site-packages\\keystoneauth1-2.9.0-py2.7.egg',
//		'C:\\Python27\\lib\\site-packages\\debtcollector-1.6.0-py2.7.egg',
//		'C:\\Python27\\lib\\site-packages\\pbr-1.10.0-py2.7.egg',
//		'C:\\Python27\\lib\\site-packages\\wrapt-1.10.8-py2.7.egg',
//		'C:\\Python27\\lib\\site-packages\\pyparsing-2.1.5-py2.7-win32.egg',
//		'C:\\Python27\\lib\\site-packages\\netifaces-0.10.4-py2.7-win32.egg',
//		'C:\\Python27\\lib\\site-packages\\netaddr-0.7.18-py2.7.egg',
//		'C:\\Python27\\lib\\site-packages\\monotonic-1.1-py2.7.egg',
//		'C:\\Python27\\lib\\site-packages\\iso8601-0.1.11-py2.7.egg',
//		'C:\\Python27\\lib\\site-packages\\funcsigs-1.0.2-py2.7.egg',
//		'C:\\Python27\\lib\\site-packages\\msgpack_python-0.4.7-py2.7-win32.egg',
//		'C:\\Python27\\lib\\site-packages\\babel-2.3.4-py2.7.egg',
//		'C:\\Python27\\lib\\site-packages\\rfc3986-0.3.1-py2.7.egg',
//		'C:\\Python27\\Lib',
//		'C:\\Python27\\libs',
//		'C:\\windows\\SYSTEM32\\python27.zip',
//		'C:\\Python27\\DLLs',
//		'C:\\Python27\\lib\\plat-win',
//		'C:\\Python27\\lib\\lib-tk',
//		'C:\\Users\\kosta\\Desktop\\test\\test\\Release',
//		'C:\\Python27',
//		'C:\\Python27\\lib\\site-packages']
//		*/
//}
//
//int main(int argc, char** argv)
//{
//	Py_SetProgramName(argv[0]);		
//
//	Py_Initialize();
//		/*
//		*** Py_Initialize()
//		파이썬 API를 사용하기 위해서는 반드시 전역 초기화를 수행하고,
//		사용을 종료한 후에는 전역 마무리를 해야 한다.
//		초기화를 수행하는 함수이다.
//		모듈을 생성, 로딩하고 정보를 수집하는 등의 전반적인 초기화를 수행한다.
//		일반적으로 초기화는 프로그램 전역에 걸쳐 한 번만 실행되어야 한다.
//		*/
//
//	printf("GetProgramName: %s\n\n", Py_GetProgramName());
//		// C:\Users\kosta\Desktop\test\Release\testPython.exe
//
//	if (Py_IsInitialized()) {
//		PySys_SetArgv(argc, argv);			
//			/*
//			*** PySys_SetArgv()
//			함수로 파이썬에게 인자를 전달할 수 있다.
//			PySys_SetArgvEx() 함수의 축약형이다.
//			마지막 updatepath의 인자를 1로 전달한다.
//			*/
//
//		printf("GetPath: %s\n\n", Py_GetPath());
//			// C:\Python27\Lib;C:\Python27\libs;C:\windows\SYSTEM32\python27.zip...(생략)
//			/*
//			*** Py_GetPath
//			파이썬이 어떤 패스를 참조하고 있는지 출력
//			시스템 값에 따라 달라질 수 있다.
//			윈도우에서는 실행 파일이 있는 디렉토리까지의 절대 경로가 출력된다.
//			*/
//
//		SysPath();
//
//		PrintMyDef();		// Calling python functions
//		printf("%d * %d = %d\n", 10, 15, Multiply(10, 15));
//		
//		Py_Finalize();
//			/*
//			*** Py_Finalize()
//			프로그램이 종료되기 전에 파이썬 마무리 함수가 반드시 한 번 불려져야 한다.
//			*/
//	}
//	return 0;
//}
