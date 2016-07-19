//#include <Python.h>
//
//void PrintMyDef()
//{
//	// 1. ���� ����� import�Ѵ�.
//	PyObject* mydef = PyImport_ImportModule("scripts.my_def");
//		/*
//		*** �ҽ� �ڵ��� ���
//		C ���� ������ �����Ǵ� ���� ��scripts����� ���丮�� �ϳ� �����,
//		�װ��� my_def.py ������ �����Ѵ�.
//		*/
//
//	// 2. ����� ����Ʈ �Ǿ ����� �����ϴ� ������Ʈ�� ������,
//	if(mydef) {
//		// �� �Լ��� �����ϴ� ������Ʈ�� ����.
//		PyObject* printHello = PyObject_GetAttrString(mydef, "PrintMyDef");
//
//		// 3. �Լ��� ������Ʈ�� �������, 
//		if(printHello) {
//			// �� �Լ��� �ҷ�����.
//			// PrintMyDef() �Լ��� None ������Ʈ�� �����Ѵ�.
//			// �̰��� C�� NULL �����Ϳʹ� �ٸ���.
//			// Ȯ���� �޸𸮸� �����ϰ� �ִ� ��ü�̴�.
//			PyObject *r = PyObject_CallFunction(printHello, NULL);
//
//			if (r == Py_None) {
//				printf("None is returned.\n");
//				Py_XDECREF(r);
//			}
//
//			// 4. �Լ��� �ҷ����� ���� ������ ������Ʈ����
//			// ���� ����(Py_XDECREF)�Ѵ�.
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
//	// 1. ���� ����� import �Ѵ�.
//	PyObject* mydef = PyImport_ImportModule("scripts.my_def");
//
//	int result;
//	// 2. ����� ����Ʈ �Ǿ ����� �����ϴ� ������Ʈ�� ������,
//	if(mydef) {
//		// �� �Լ��� �����ϴ� ������Ʈ�� ����.
//		PyObject* multiply = PyObject_GetAttrString(mydef, "Multiply");
//		
//		// 3. �Լ��� ������Ʈ�� �������,
//		if(multiply) {
//			// �� �Լ��� �ҷ�����.
//			// ���� 2���� ����ְ� ����� �޾ƿ´�.
//			PyObject *r = PyObject_CallFunction(multiply, "ii", x, y);
//			if(r) {
//				result = (int)PyInt_AS_LONG(r);
//				Py_XDECREF(r);
//			}
//
//			// 4. �Լ��� �ҷ����� ���� ������ ������Ʈ����
//			// ���� ����(Py_XDECREF)�Ѵ�.
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
//		���̽� ��ũ��Ʈ�� � ����� ��������(import) ���ؼ���
//		����� ��θ� �̸� �˰� �־�� �Ѵ�.
//		�� ��δ� ���� sys.path�� ��ϵȴ�.
//		PySys_SetArgv() �Լ��� ������ �شٸ� ���� ��ũ��Ʈ�� ����Ǵ� ��θ� ��������
//		sys.path�� ��ϵȴ�.
//		�׷��� �ٸ� ��ο� �ִ� ����� ���̽� ��ũ��Ʈ�� ����ϰ� �ʹٸ�
//		�ݵ�� �� ��θ� �Է��� �־�� �Ѵ�.
//
//		sys.path�� ȣ���ؼ� ��ΰ� ����� �������� Ȯ���Ѵ�.
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
//		���̽� API�� ����ϱ� ���ؼ��� �ݵ�� ���� �ʱ�ȭ�� �����ϰ�,
//		����� ������ �Ŀ��� ���� �������� �ؾ� �Ѵ�.
//		�ʱ�ȭ�� �����ϴ� �Լ��̴�.
//		����� ����, �ε��ϰ� ������ �����ϴ� ���� �������� �ʱ�ȭ�� �����Ѵ�.
//		�Ϲ������� �ʱ�ȭ�� ���α׷� ������ ���� �� ���� ����Ǿ�� �Ѵ�.
//		*/
//
//	printf("GetProgramName: %s\n\n", Py_GetProgramName());
//		// C:\Users\kosta\Desktop\test\Release\testPython.exe
//
//	if (Py_IsInitialized()) {
//		PySys_SetArgv(argc, argv);			
//			/*
//			*** PySys_SetArgv()
//			�Լ��� ���̽㿡�� ���ڸ� ������ �� �ִ�.
//			PySys_SetArgvEx() �Լ��� ������̴�.
//			������ updatepath�� ���ڸ� 1�� �����Ѵ�.
//			*/
//
//		printf("GetPath: %s\n\n", Py_GetPath());
//			// C:\Python27\Lib;C:\Python27\libs;C:\windows\SYSTEM32\python27.zip...(����)
//			/*
//			*** Py_GetPath
//			���̽��� � �н��� �����ϰ� �ִ��� ���
//			�ý��� ���� ���� �޶��� �� �ִ�.
//			�����쿡���� ���� ������ �ִ� ���丮������ ���� ��ΰ� ��µȴ�.
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
//			���α׷��� ����Ǳ� ���� ���̽� ������ �Լ��� �ݵ�� �� �� �ҷ����� �Ѵ�.
//			*/
//	}
//	return 0;
//}
