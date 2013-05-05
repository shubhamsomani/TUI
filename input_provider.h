// This header file makes use of previously made text parser for providing input to the TUI.
// Since the text parser was in python and ncurses is in c. Hence I have made use of the Python/C API.
// Author : Shubham Somani

# include</home/anomaly/python/include/python2.7/Python.h> //Path needs to be changed according to your system.

int return_parameters(char ** input[50]);

int return_parameters(char ** input[50])
{
//Initializing the Python/C API
Py_Initialize();

int len=0;

PyObject *modname, *mod,*mdict,*func,*func2,*result,*item,*item2; 

modname = PyString_FromString("config_parser");

if (modname == NULL)
{
	Py_Exit(0);
	return 0;
}

//Importing the configuration parser 
mod = PyImport_Import(modname);

if(mod)
{
	mdict = PyModule_GetDict(mod);

    //Calling the write function of the configuration parser.
	func = PyDict_GetItemString(mdict, "write"); 
	if (func)
	{
	      PyObject_CallObject(func,0);
              
	}
	
	//Calling the read function of the configuration parser.
	func2 = PyDict_GetItemString(mdict, "read"); 
	if(func&&func2)
	{ 
	      result=PyObject_CallObject(func2,0);
	      len=PyList_GET_SIZE(result);
	      int i=0;
	      int init=0;
	      
	      //Allocating space of 200 elements for each section
	      for(init=0;init<len;init++)
	      {
    		 input[init]=(char**)malloc(200*sizeof(*input));
	      }
          
          //Reading elements from a python list and transferring it into a charater array
	      for(i=0;i<len;i++)
	      {
              PyObject* section=PySequence_Fast_GET_ITEM(result, i);	
              int section_len=PyList_GET_SIZE(section);
		      int j=0;
              for(j=0;j<section_len;j++)
		      {
                       PyObject* item1=PySequence_Fast_GET_ITEM(section, j);
                       input[i][j]=PyBytes_AsString(item1);
              }
	          input[i][j]="end";
	      }
	      
	      //PyRun_SimpleString("print 'Success'");	 

	}

return len;

}
else
{
	PyRun_SimpleString("print 'Error while importing Python module'");
}

//freeing memory
	
Py_XDECREF(mod);
Py_XDECREF(modname);
Py_Finalize();

}

