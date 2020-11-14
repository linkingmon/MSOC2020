/**********
Copyright (c) 2018, Xilinx, Inc.
All rights reserved.
Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.
3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**********/

/*******************************************************************************
** HOST Code
*******************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

#include <CL/cl.h>

#include "help_functions.h"
#include "kernel.h"

#define ALL_MESSAGES

// ********************************************************************************** //
// ---------------------------------------------------------------------------------- //
//                          M A I N    F U N C T I O N                                //
// ---------------------------------------------------------------------------------- //
// ********************************************************************************** //

int main(int argc, char* argv[])
{
	cout << endl;


	// ============================================================================
	// Step 1: Check Command Line Arguments
	// ============================================================================
	//    o) argv[1] Platfrom Vendor
	//    o) argv[2] Device Name
	//    o) argv[3] XCLBIN file
	// ============================================================================
	#ifdef ALL_MESSAGES
	cout << "HOST-Info: ============================================================= " << endl;
	cout << "HOST-Info: (Step 1) Check Command Line Arguments                      " << endl;
	cout << "HOST-Info: ============================================================= " << endl;
	#endif

	if (argc != 4)
	{
		cout << "HOST-Error: Incorrect command line syntax " << endl;
		cout << "HOST-Info:  Usage: " << argv[0] << " <Platform_Vendor> <Device_Name> <XCLBIN_File>  <Test Vectors Size>" << endl << endl;
		return EXIT_FAILURE;
	} 
 
	const char* Target_Platform_Vendor   = argv[1];
	const char* Target_Device_Name       = argv[2];
	const char* xclbinFilename           = argv[3];
	cout << "HOST-Info: Platform_Vendor   : " << Target_Platform_Vendor << endl;
	cout << "HOST-Info: Device_Name       : " << Target_Device_Name << endl;
	cout << "HOST-Info: XCLBIN_file       : " << xclbinFilename << endl;


	// ============================================================================
	// Step 2: Detect Target Platform and Target Device in a system. 
	//         Create Context and Command Queue.
	// ============================================================================
	// Variables:
	//   o) Target_Platform_Vendor[] - defined as main() input argument 
	//   o) Target_Device_Name[]     - defined as main() input argument
	// 
	// After that
	//   o) Create a Context
	//   o) Create a Command Queue
	// ============================================================================
	cout << endl;
	#ifdef ALL_MESSAGES
	cout << "HOST-Info: ============================================================= " << endl;
	cout << "HOST-Info: (Step 2) Detect Target Platform and Target Device in a system " << endl;
	cout << "HOST-Info:          Create Context and Command Queue                     " << endl;
	cout << "HOST-Info: ============================================================= " << endl;
	#endif

	cl_uint         ui;

	cl_platform_id      *Platform_IDs;
	cl_uint             Nb_Of_Platforms;
	cl_platform_id      Target_Platform_ID;
	bool                Platform_Detected;
	char                *platform_info;

	cl_device_id        *Device_IDs;
	cl_uint             Nb_Of_Devices;
	cl_device_id        Target_Device_ID;
	bool                Device_Detected;
	char                *device_info;

	cl_context          Context;
	cl_command_queue    Command_Queue;

	cl_int              errCode;
	size_t              size;

	// ------------------------------------------------------------------------------------
	// Step 2.1: Get All PLATFORMS, then search for Target_Platform_Vendor (CL_PLATFORM_VENDOR)
	// ------------------------------------------------------------------------------------
	
	// Get the number of platforms
	// ..................................................
	errCode = clGetPlatformIDs(0, NULL, &Nb_Of_Platforms);
	if (errCode != CL_SUCCESS || Nb_Of_Platforms <= 0) {
		cout << endl << "HOST-Error: Failed to get the number of available platforms" << endl << endl;
		return EXIT_FAILURE;
	}

	#ifdef ALL_MESSAGES
	cout << "HOST-Info: Number of detected platforms : " << Nb_Of_Platforms << endl;
	#endif

	// Allocate memory to store platforms
	// ..................................................
	Platform_IDs = new cl_platform_id[Nb_Of_Platforms];
	if (!Platform_IDs) {
		cout << endl << "HOST-Error: Out of Memory during memory allocation for Platform_IDs" << endl << endl;
		return EXIT_FAILURE;
	}

	// Get and store all PLATFORMS
	// ..................................................
	errCode = clGetPlatformIDs(Nb_Of_Platforms, Platform_IDs, NULL);
	if (errCode != CL_SUCCESS) {
		cout << endl << "HOST-Error: Failed to get the available platforms" << endl << endl;
		return EXIT_FAILURE;
	}
 
	// Search for Platform (ex: Xilinx) using: CL_PLATFORM_VENDOR = Target_Platform_Vendor
	// ....................................................................................
	Platform_Detected = false;
	for (ui = 0; ui < Nb_Of_Platforms; ui++) {

		errCode = clGetPlatformInfo(Platform_IDs[ui], CL_PLATFORM_VENDOR, 0, NULL, &size);
		if (errCode != CL_SUCCESS) {
			cout << endl << "HOST-Error: Failed to get the size of the Platofrm parameter " << "CL_PLATFORM_VENDOR" << " value " << endl << endl;
			return EXIT_FAILURE;
		}

		platform_info = new char[size];
		if (!platform_info) {
			cout << endl << "HOST-Error: Out of Memory during memory allocation for Platform Parameter " << "CL_PLATFORM_VENDOR" << endl << endl;
			return EXIT_FAILURE;
		}

		errCode = clGetPlatformInfo(Platform_IDs[ui], CL_PLATFORM_VENDOR, size, platform_info , NULL);
		if (errCode != CL_SUCCESS) {
			cout << endl << "HOST-Error: Failed to get the " << "CL_PLATFORM_VENDOR" << " platform info" << endl << endl;
			return EXIT_FAILURE;
		}

		// Check if the current platform matches Target_Platform_Vendor
		// .............................................................
		if (strcmp(platform_info, Target_Platform_Vendor) == 0) {
			Platform_Detected        = true;
			Target_Platform_ID       = Platform_IDs[ui];
			#ifdef ALL_MESSAGES
			cout << "HOST-Info: Selected platform            : " << Target_Platform_Vendor << endl << endl;
			#endif
		}
	}

	if (Platform_Detected == false) {
		cout << endl << "HOST-Error: Failed to get detect " << Target_Platform_Vendor << " platform" << endl << endl;
		return EXIT_FAILURE;
	}


	// ------------------------------------------------------------------------------------
	// Step 2.2:  Get All Devices for selected platform Target_Platform_ID
	//            then search for Xilinx platform (CL_DEVICE_NAME = Target_Device_Name)
	// ------------------------------------------------------------------------------------

	// Get the Number of Devices
	// ............................................................................
	errCode = clGetDeviceIDs(Target_Platform_ID, CL_DEVICE_TYPE_ALL, 0, NULL, &Nb_Of_Devices);
	if (errCode != CL_SUCCESS) {
		cout << endl << "HOST-Error: Failed to get the number of available Devices" << endl << endl;
		return EXIT_FAILURE;
	}
	#ifdef ALL_MESSAGES
	cout << "HOST-Info: Number of available devices  : " << Nb_Of_Devices << endl;
	#endif

	Device_IDs = new cl_device_id[Nb_Of_Devices];
	if (!Device_IDs) {
		cout << endl << "HOST-Error: Out of Memory during memory allocation for Device_IDs" << endl << endl;
		return EXIT_FAILURE;
	}

	errCode = clGetDeviceIDs(Target_Platform_ID, CL_DEVICE_TYPE_ALL, Nb_Of_Devices, Device_IDs, NULL);
	if (errCode != CL_SUCCESS) {
		cout << endl << "HOST-Error: Failed to get available Devices" << endl << endl;
		return EXIT_FAILURE;
	}

	// Search for CL_DEVICE_NAME = Target_Device_Name
	// ............................................................................
	Device_Detected = false;
	for (ui = 0; ui < Nb_Of_Devices; ui++) {
		errCode = clGetDeviceInfo(Device_IDs[ui], CL_DEVICE_NAME, 0, NULL, &size);
		if (errCode != CL_SUCCESS) {
			cout << endl << "HOST-Error: Failed to get the size of the Device parameter value " << "CL_DEVICE_NAME" << endl << endl;
			return EXIT_FAILURE;
		}
				
		device_info = new char[size];
		if (!device_info) {
			cout << endl << "HOST-Error: Out of Memory during memory allocation for Device parameter " << "CL_DEVICE_NAME" << " value " << endl << endl;
			return EXIT_FAILURE;
		}
				
		errCode = clGetDeviceInfo(Device_IDs[ui], CL_DEVICE_NAME, size, device_info, NULL);
		if (errCode != CL_SUCCESS) {
			cout << endl << "HOST-Error: Failed to get the " << "CL_DEVICE_NAME" << " device info" << endl << endl;
			return EXIT_FAILURE;
		}

		// Check if the current device matches Target_Device_Name
		// ............................................................................
		if (strcmp(device_info, Target_Device_Name) == 0) {
			Device_Detected        = true;
			Target_Device_ID       = Device_IDs[ui];
		}
	}

	if (Device_Detected == false) {
		cout << endl << "HOST-Error: Failed to get detect " << Target_Device_Name << " device" << endl << endl;
		return EXIT_FAILURE;
	} else {
		#ifdef ALL_MESSAGES
		cout << "HOST-Info: Selected device              : " << Target_Device_Name << endl << endl;
		#endif
	}

	// ------------------------------------------------------------------------------------
	// Step 2.3: Create Context
	// ------------------------------------------------------------------------------------
	#ifdef ALL_MESSAGES
	cout << "HOST-Info: Creating Context ... " << endl;
	#endif
	Context = clCreateContext(0, 1, &Target_Device_ID, NULL, NULL, &errCode);
	if (errCode != CL_SUCCESS) {
		cout << endl << "HOST-Error: Failed to create a Context" << endl << endl;
		return EXIT_FAILURE;
	}

	// ------------------------------------------------------------------------------------
	// Step 2.4: Create Command Queue (commands are executed in-order)
	// ------------------------------------------------------------------------------------
	#ifdef ALL_MESSAGES
	cout << "HOST-Info: Creating Command Queue ... " << endl;
	#endif
	Command_Queue = clCreateCommandQueue(Context, Target_Device_ID, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_PROFILING_ENABLE, &errCode);
	if (errCode != CL_SUCCESS) {
		cout << endl << "HOST-Error: Failed to create a Command Queue" << endl << endl;
		return EXIT_FAILURE;
	}

	// ============================================================================
	// Step 3: Create Program and Kernel
	// ============================================================================
	//   o) Create a Program from a Binary File and Build it
	//   o) Create a Kernel
	// ============================================================================
	#ifdef ALL_MESSAGES
	cout << endl;
	cout << "HOST-Info: ============================================================= " << endl;
	cout << "HOST-Info: (Step 3) Create Program and Kernels                           " << endl;
	cout << "HOST-Info: ============================================================= " << endl;
	#endif

	// ------------------------------------------------------------------
	// Step 3.1: Load Binary File from a disk to Memory
	// ------------------------------------------------------------------
	unsigned char *xclbin_Memory;
	int program_length;
	
	#ifdef ALL_MESSAGES
	cout << "HOST-Info: Loading " << xclbinFilename << " binary file to memory ..." << endl;
	#endif

	program_length = loadFile2Memory(xclbinFilename, (char **) &xclbin_Memory);
	if (program_length < 0) {
		cout << endl << "HOST-Error: Failed to load " << xclbinFilename << " binary file to memory" << endl << endl;
		return EXIT_FAILURE;
	}

	// ------------------------------------------------------------
	// Step 3.2: Create a program using a Binary File
	// ------------------------------------------------------------
	size_t     Program_Length_in_Bytes;
	cl_program Program;
	cl_int     Binary_Status;
	
	#ifdef ALL_MESSAGES
	cout << "HOST-Info: Creating Program with Binary ..." << endl;
	#endif
	Program_Length_in_Bytes = program_length;
	Program = clCreateProgramWithBinary(Context, 1, &Target_Device_ID, &Program_Length_in_Bytes, 
                                        (const unsigned char **) &xclbin_Memory, &Binary_Status, &errCode);
	if (errCode != CL_SUCCESS) {
		cout << endl << "HOST-Error: Failed to create a Program from a Binary" << endl << endl;
		return EXIT_FAILURE;
	}

	// ----------------------------------------------------------------------
	// Step 3.3: Build (compiles and links) a program executable from binary
	// ----------------------------------------------------------------------
	#ifdef ALL_MESSAGES
	cout << "HOST-Info: Building the Program ..." << endl;
	#endif

	errCode = clBuildProgram(Program, 1, &Target_Device_ID, NULL, NULL, NULL);
	if (errCode != CL_SUCCESS) {
		cout << endl << "HOST-Error: Failed to build a Program Executable" << endl << endl;
		return EXIT_FAILURE;
	}

	// -------------------------------------------------------------
	// Step 3.4: Create a Kernels
	// -------------------------------------------------------------
	cl_kernel K_KVConstAdd, K_KpB, K_KA, K_KB, K_KCalc;

	#ifdef ALL_MESSAGES
	cout << "HOST-Info: Creating a Kernel: KVConstAdd ..." << endl;
	#endif
	K_KVConstAdd = clCreateKernel(Program, "KVConstAdd", &errCode);
	if (errCode != CL_SUCCESS) {
		cout << endl << "HOST-Error: Failed to create K_KVConstAdd" << endl << endl;
		return EXIT_FAILURE;
	}

	#ifdef ALL_MESSAGES
	cout << "HOST-Info: Creating a Kernel: K_KpB ..." << endl;
	#endif
	K_KpB = clCreateKernel(Program, "KpB", &errCode);
	if (errCode != CL_SUCCESS) {
		cout << endl << "HOST-Error: Failed to create K_KpB" << endl << endl;
		return EXIT_FAILURE;
	}

	#ifdef ALL_MESSAGES
	cout << "HOST-Info: Creating a Kernel: K_KA ..." << endl;
	#endif
	K_KA = clCreateKernel(Program, "KA", &errCode);
	if (errCode != CL_SUCCESS) {
		cout << endl << "HOST-Error: Failed to create K_KA" << endl << endl;
		return EXIT_FAILURE;
	}

	#ifdef ALL_MESSAGES
	cout << "HOST-Info: Creating a Kernel: K_KB ..." << endl;
	#endif
	K_KB = clCreateKernel(Program, "KB", &errCode);
	if (errCode != CL_SUCCESS) {
		cout << endl << "HOST-Error: Failed to create K_KB" << endl << endl;
		return EXIT_FAILURE;
	}


	#ifdef ALL_MESSAGES
	cout << "HOST-Info: Creating a Kernel: K_KCalc ..." << endl;
	#endif
	K_KCalc = clCreateKernel(Program, "KCalc", &errCode);
	if (errCode != CL_SUCCESS) {
		cout << endl << "HOST-Error: Failed to create K_KCalc" << endl << endl;
		return EXIT_FAILURE;
	}

	// ================================================================
	// Step 4: Prepare Data to Run Kernel
	// ================================================================
	//   o) Generate data for DataIn_1 array
	//   o) Generate data for DataIn_2 array
	//   o) Generate data for DataIn_3 array
	//   o) Allocate Memory to store the results: RES array
	//   o) Create Buffers in Global Memory to store data
	// ================================================================
	int *DataIn_1, *DataIn_2, *DataIn_3, *RES;

	#ifdef ALL_MESSAGES
	cout << endl;
	cout << "HOST-Info: ============================================================= " << endl;
	cout << "HOST-Info: (Step 4) Prepare Data to Run Kernels                           " << endl;
	cout << "HOST-Info: ============================================================= " << endl;
	#endif

	// ------------------------------------------------------------------
	// Step 4.1: Generate data for DataIn_1 array
	//           Generate data for DataIn_2 array
	//           Generate data for DataIn_3 array
	//           Allocate Memory to store the results: RES array
	// ------------------------------------------------------------------
	cl_uint CONST_arg = 5;

	void *ptr=nullptr;
	int Values_Period = 3;

	cout << "HOST-Info: Generating data for DataIn_1 ... ";
	if (posix_memalign(&ptr,4096,SIZE_DataIn_1*sizeof(int))) {
		cout << endl << "HOST-Error: Out of Memory during memory allocation for DataIn_1 array" << endl << endl;
		return EXIT_FAILURE;
	}
	DataIn_1 = reinterpret_cast<int*>(ptr);
	gen_int_values(DataIn_1,SIZE_DataIn_1, Values_Period);
	cout << "Generated " << SIZE_DataIn_1 << " values" << endl;


	cout << "HOST-Info: Generating data for DataIn_2 ... ";
	if (posix_memalign(&ptr,4096,SIZE_DataIn_2*sizeof(int))) {
		cout << endl << "HOST-Error: Out of Memory during memory allocation for DataIn_2 array" << endl << endl;
		return EXIT_FAILURE;
	}
	DataIn_2 = reinterpret_cast<int*>(ptr);
	gen_int_values(DataIn_2,SIZE_DataIn_2, Values_Period);
	cout << "Generated " << SIZE_DataIn_2 << " values" << endl;


	cout << "HOST-Info: Generating data for DataIn_3 ... ";
	DataIn_3 = new int[SIZE_DataIn_3];
	gen_int_values(DataIn_3,SIZE_DataIn_3, Values_Period);
	cout << "Generated " << SIZE_DataIn_3 << " values" << endl;


	cout << "HOST-Info: Allocating memory for RES    ... ";
	if (posix_memalign(&ptr,4096,SIZE_RES*sizeof(int))) {
		cout << endl << "HOST-Error: Out of Memory during memory allocation for RES array" << endl << endl;
		return EXIT_FAILURE;
	}
	cout << "Allocated" << endl;
	RES = reinterpret_cast<int*>(ptr);

	cout << endl;

	// ------------------------------------------------------------------
	// Step 4.2: Create Buffers in Global Memory to store data
	//             o) GlobMem_BUF_DataIn_1 - stores DataIn_1 (R/W)
	//             o) GlobMem_BUF_DataIn_2 - stores DataIn_2 (R)
	//             o) GlobMem_BUF_DataIn_3 - stores DataIn_3 (R)
	//             o) GlobMem_BUF_KpB      - stores Results from K_KpB (R/W)
	//             o) GlobMem_BUF_KA       - stores Results from K_KA  (R/W)
	//             o) GlobMem_BUF_KB       - stores Results from K_KB  (R/W)
	//             o) GlobMem_BUF_RES      - stores RES (W)
	// ------------------------------------------------------------------
	#ifdef ALL_MESSAGES
	cout << "HOST-Info: Allocating buffers in Global Memory to store Input and Output Data ..." << endl;
	#endif
	cl_mem	GlobMem_BUF_DataIn_1, GlobMem_BUF_DataIn_2, GlobMem_BUF_DataIn_3,
			GlobMem_BUF_KpB, GlobMem_BUF_KA, GlobMem_BUF_KB,
			GlobMem_BUF_RES;

	// Allocate Global Memory for GlobMem_BUF_DataIn_1
	// .......................................................
	GlobMem_BUF_DataIn_1 = clCreateBuffer(Context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, SIZE_DataIn_1 * sizeof(int), DataIn_1, &errCode);
	if (errCode != CL_SUCCESS) {
		cout << endl << "Host-Error: Failed to allocate Global Memory for GlobMem_BUF_DataIn_1" << endl << endl;
		return EXIT_FAILURE;
	}

	// Allocate Global Memory for GlobMem_BUF_DataIn_2
	// .......................................................
	GlobMem_BUF_DataIn_2 = clCreateBuffer(Context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, SIZE_DataIn_2 * sizeof(int), DataIn_2, &errCode);
	if (errCode != CL_SUCCESS) {
		cout << endl << "Host-Error: Failed to allocate Global Memory for GlobMem_BUF_DataIn_2" << endl << endl;
		return EXIT_FAILURE;
	}

	// Allocate Global Memory for GlobMem_BUF_DataIn_3
	// .......................................................
	GlobMem_BUF_DataIn_3 = clCreateBuffer(Context, CL_MEM_READ_ONLY, SIZE_DataIn_3 * sizeof(int), NULL, &errCode);
	if (errCode != CL_SUCCESS) {
		cout << endl << "Host-Error: Failed to allocate Global Memory for GlobMem_BUF_DataIn_3" << endl << endl;
		return EXIT_FAILURE;
	}

	// Allocate Global Memory for GlobMem_BUF_KpB
	// .......................................................
	GlobMem_BUF_KpB = clCreateBuffer(Context, CL_MEM_READ_WRITE, SIZE_BUF_KpB * sizeof(int), NULL, &errCode);
	if (errCode != CL_SUCCESS) {
		cout << endl << "Host-Error: Failed to allocate Global Memory for GlobMem_BUF_KpB" << endl << endl;
		return EXIT_FAILURE;
	}

	// Allocate Global Memory for GlobMem_BUF_KA
	// .......................................................
	GlobMem_BUF_KA = clCreateBuffer(Context, CL_MEM_READ_WRITE, SIZE_BUF_KA * sizeof(int), NULL, &errCode);
	if (errCode != CL_SUCCESS) {
		cout << endl << "Host-Error: Failed to allocate Global Memory for GlobMem_BUF_KA" << endl << endl;
		return EXIT_FAILURE;
	}

	// Allocate Global Memory for GlobMem_BUF_KB
	// .......................................................
	GlobMem_BUF_KB = clCreateBuffer(Context, CL_MEM_READ_WRITE, SIZE_BUF_KB * sizeof(int), NULL, &errCode);
	if (errCode != CL_SUCCESS) {
		cout << endl << "Host-Error: Failed to allocate Global Memory for GlobMem_BUF_KB" << endl << endl;
		return EXIT_FAILURE;
	}

	// Allocate Global Memory for GlobMem_BUF_RES
	// .......................................................
	GlobMem_BUF_RES = clCreateBuffer(Context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, SIZE_RES * sizeof(int), RES, &errCode);
	if (errCode != CL_SUCCESS) {
		cout << endl << "Host-Error: Failed to allocate Global Memory for GlobMem_BUF_RES" << endl << endl;
		return EXIT_FAILURE;
	}

	// ============================================================================
	// Step 5: Set Kernel Arguments and Run the Application
	//         o) Set Kernel Arguments
	// 				----------------------------------------------------
	// 				 Kernel	  		Argument Nb		Description
	// 				----------------------------------------------------
	//  			 K_KVConstAdd	0				CONST_arg
	//				 K_KVConstAdd	1				GlobMem_BUF_DataIn_1
	//
	//  			 K_KA			0				GlobMem_BUF_DataIn_1
	//  			 K_KA			1				GlobMem_BUF_KA
	//
	//  			 K_KpB			0				GlobMem_BUF_DataIn_2
	//  			 K_KpB			1				GlobMem_BUF_DataIn_3
	//  			 K_KpB			2				GlobMem_BUF_KpB
	//
	//  			 K_KB			0				GlobMem_BUF_KpB
	//  			 K_KB			1				GlobMem_BUF_KB
	//
	//  			 K_KCalc		0				GlobMem_BUF_KA
	//  			 K_KCalc		1				GlobMem_BUF_KB
	//  			 K_KCalc		2				GlobMem_BUF_RES
	// 				----------------------------------------------------
	//         o) Copy Input Data from Host to Global Memory
	//         o) Submit Kernels for Execution
	//         o) Copy Results from Global Memory to Host
	// ============================================================================
	int Nb_Of_Mem_Events = 8,
		Nb_Of_Exe_Events = 5;

	cl_event Mem_op_event[Nb_Of_Mem_Events],
	          K_exe_event[Nb_Of_Exe_Events];


	#ifdef ALL_MESSAGES
	cout << endl;
	cout << "HOST-Info: ============================================================= " << endl;
	cout << "HOST-Info: (Step 5) Run Application                                      " << endl;
	cout << "HOST-Info: ============================================================= " << endl;
	#endif


	// ----------------------------------------
	// Step 5.1: Set Kernel Arguments
	// ----------------------------------------
	#ifdef ALL_MESSAGES
	cout << "HOST-Info: Setting Kernel arguments ..." << endl;
	#endif
	errCode  = false;

	errCode |= clSetKernelArg(K_KVConstAdd,  0, sizeof(cl_uint), &CONST_arg);
	errCode |= clSetKernelArg(K_KVConstAdd,  1, sizeof(cl_mem),  &GlobMem_BUF_DataIn_1);

	errCode |= clSetKernelArg(K_KA,          0, sizeof(cl_mem),  &GlobMem_BUF_DataIn_1);
	errCode |= clSetKernelArg(K_KA,          1, sizeof(cl_mem),  &GlobMem_BUF_KA);

	errCode |= clSetKernelArg(K_KpB,         0, sizeof(cl_mem),  &GlobMem_BUF_DataIn_2);
	errCode |= clSetKernelArg(K_KpB,         1, sizeof(cl_mem),  &GlobMem_BUF_DataIn_3);
	errCode |= clSetKernelArg(K_KpB,         2, sizeof(cl_mem),  &GlobMem_BUF_KpB);

	errCode |= clSetKernelArg(K_KB,          0, sizeof(cl_mem),  &GlobMem_BUF_KpB);
	errCode |= clSetKernelArg(K_KB,          1, sizeof(cl_mem),  &GlobMem_BUF_KB);

	errCode |= clSetKernelArg(K_KCalc,       0, sizeof(cl_mem),  &GlobMem_BUF_KA);
	errCode |= clSetKernelArg(K_KCalc,       1, sizeof(cl_mem),  &GlobMem_BUF_KB);
	errCode |= clSetKernelArg(K_KCalc,       2, sizeof(cl_mem),  &GlobMem_BUF_RES);

	if (errCode != CL_SUCCESS) {
		cout << endl << "Host-ERROR: Failed to set Kernel arguments" << endl << endl;
		return EXIT_FAILURE;
	}

	// ------------------------------------------------------
	// Step 5.2: Copy Input data from Host to Global Memory
	// ------------------------------------------------------
	#ifdef ALL_MESSAGES
	cout << "HOST_Info: Copy Input data to Global Memory ..." << endl;
	#endif
	errCode = clEnqueueMigrateMemObjects(Command_Queue, 1, &GlobMem_BUF_DataIn_1, 0, 0, NULL, &Mem_op_event[0]);
	if (errCode != CL_SUCCESS) {
		cout << endl << "Host-Error: Failed to write DataIn_1 to GlobMem_BUF_DataIn_1" << endl << endl;
		return EXIT_FAILURE;
	}

	errCode = clEnqueueMigrateMemObjects(Command_Queue, 1, &GlobMem_BUF_DataIn_2, 0, 0, NULL, &Mem_op_event[1]);
	if (errCode != CL_SUCCESS) {
		cout << endl << "Host-Error: Failed to write DataIn_2 to GlobMem_BUF_DataIn_2" << endl << endl;
		return EXIT_FAILURE;
	}

	errCode = clEnqueueWriteBuffer(Command_Queue, GlobMem_BUF_DataIn_3, 0, 0, SIZE_DataIn_3 * sizeof(int), DataIn_3, 0, NULL, &Mem_op_event[2]);
	if (errCode != CL_SUCCESS) {
		cout << endl << "Host-Error: Failed to write DataIn_3 to GlobMem_BUF_DataIn_3" << endl << endl;
		return EXIT_FAILURE;
	}

	// --------------------------------------------------------


	errCode = clEnqueueMigrateMemObjects(Command_Queue, 1, &GlobMem_BUF_KpB, CL_MIGRATE_MEM_OBJECT_CONTENT_UNDEFINED, 0, NULL, &Mem_op_event[3]);
	if (errCode != CL_SUCCESS) {
		cout << endl << "Host-Error: Failed Migrate GlobMem_BUF_KpB without migrating content" << endl << endl;
		return EXIT_FAILURE;
	}

	errCode = clEnqueueMigrateMemObjects(Command_Queue, 1, &GlobMem_BUF_KA, CL_MIGRATE_MEM_OBJECT_CONTENT_UNDEFINED, 0, NULL, &Mem_op_event[4]);
	if (errCode != CL_SUCCESS) {
		cout << endl << "Host-Error: Failed Migrate GlobMem_BUF_KA without migrating content" << endl << endl;
		return EXIT_FAILURE;
	}

	errCode = clEnqueueMigrateMemObjects(Command_Queue, 1, &GlobMem_BUF_KB, CL_MIGRATE_MEM_OBJECT_CONTENT_UNDEFINED, 0, NULL, &Mem_op_event[5]);
	if (errCode != CL_SUCCESS) {
		cout << endl << "Host-Error: Failed Migrate GlobMem_BUF_KB without migrating content" << endl << endl;
		return EXIT_FAILURE;
	}

	errCode = clEnqueueMigrateMemObjects(Command_Queue, 1, &GlobMem_BUF_RES, CL_MIGRATE_MEM_OBJECT_CONTENT_UNDEFINED, 0, NULL, &Mem_op_event[6]);
	if (errCode != CL_SUCCESS) {
		cout << endl << "Host-Error: Failed Migrate GlobMem_BUF_RES without migrating content" << endl << endl;
		return EXIT_FAILURE;
	}

	// --------------------------------------------------------

	errCode = clEnqueueBarrierWithWaitList (Command_Queue, 0, NULL, NULL);
	if (errCode != CL_SUCCESS) {
		cout << endl << "HOST-Error: Failed to Submit BarrierWithWaitList" << endl << endl;
		return EXIT_FAILURE;
	}

	// ----------------------------------------
	// Step 5.3: Submit Kernels for Execution
	// ----------------------------------------

	cout << "HOST-Info: Submitting Kernel K_KVConstAdd ..." << endl;

	errCode = clEnqueueTask(Command_Queue, K_KVConstAdd, 0, NULL, &K_exe_event[0]);
	if (errCode != CL_SUCCESS) {
		cout << endl << "HOST-Error: Failed to submit K_KVConstAdd" << endl << endl;
		return EXIT_FAILURE;
	}

	cout << "HOST-Info: Submitting Kernel K_KA ..." << endl;

	errCode = clEnqueueTask(Command_Queue, K_KA, 1, &K_exe_event[0], &K_exe_event[2]);

	if (errCode != CL_SUCCESS) {
		cout << endl << "HOST-Error: Failed to submit K_KA" << endl << endl;
		return EXIT_FAILURE;
	}

	cout << "HOST-Info: Submitting Kernel K_KpB ..." << endl;

	errCode = clEnqueueTask(Command_Queue, K_KpB, 1, &K_exe_event[2], &K_exe_event[1]);
	if (errCode != CL_SUCCESS) {
		cout << endl << "HOST-Error: Failed to submit K_KpB" << endl << endl;
		return EXIT_FAILURE;
	}

	cout << "HOST-Info: Submitting Kernel K_KB ..." << endl;

	errCode = clEnqueueTask(Command_Queue, K_KB, 1, &K_exe_event[1], &K_exe_event[3]);
	if (errCode != CL_SUCCESS) {
		cout << endl << "HOST-Error: Failed to submit K_KB" << endl << endl;
		return EXIT_FAILURE;
	}

	cout << "HOST-Info: Submitting Kernel K_KCalc ..." << endl;

	errCode = clEnqueueTask(Command_Queue, K_KCalc, 2, &K_exe_event[2], &K_exe_event[4]);
	if (errCode != CL_SUCCESS) {
		cout << endl << "HOST-Error: Failed to submit K_KCalc" << endl << endl;
		return EXIT_FAILURE;
	}

	// ---------------------------------------------------------
	// Step 5.4: Submit Copy Results from Global Memory to Host
	// ---------------------------------------------------------
	#ifdef ALL_MESSAGES
	cout << "HOST_Info: Submitting Copy Results data from Global Memory to Host ..." << endl;
	#endif

	errCode = clEnqueueMigrateMemObjects(Command_Queue, 1, &GlobMem_BUF_RES, CL_MIGRATE_MEM_OBJECT_HOST, 1, &K_exe_event[4], &Mem_op_event[7]);
	if (errCode != CL_SUCCESS) {
		cout << endl << "Host-Error: Failed to submit Copy Results from GlobMem_BUF_RES to RES" << endl << endl;
		return EXIT_FAILURE;
	}

	cout << endl << "HOST_Info: Waiting for application to be completed ..." << endl;
	clFinish(Command_Queue);


	// ============================================================================
	// Step 6: Processing Output Results
	//         o) Store output results to a RES.txt file
	//         o) Check correctness of the output results
	// ============================================================================
	#ifdef ALL_MESSAGES
	cout << endl;
	cout << "HOST-Info: ============================================================= " << endl;
	cout << "HOST-Info: (Step 6) Store and Check the Output Results                   " << endl;
	cout << "HOST-Info: ============================================================= " << endl;
	#endif

	// ------------------------------------------------------
	// Step 6.1: Store output Result to the RES.txt file
	// ------------------------------------------------------
	char Output_File_Name[] = "RES.txt";
	cout << "HOST_Info: Store output results in: " << Output_File_Name << endl;

	fstream RES_File;


	RES_File.open(Output_File_Name,ios::out);
	if (! RES_File.is_open()) {
		cout << endl << "HOST-Error: Failed to open the " << Output_File_Name << " file for write" << endl << endl;
		return EXIT_FAILURE;
	}

	for (int i=0; i < SIZE_RES; i++) {
		RES_File << RES[i] << endl;
	}
	RES_File.close();

	// ------------------------------------------------------
	// Step 6.2: Check correctness of the output results
	// ------------------------------------------------------
	bool error_detected = false;
	error_detected = check_output_results (DataIn_1, DataIn_2, DataIn_3, CONST_arg, RES);

	if (error_detected == false) {
		cout << "Host-Info: Test Successful" << endl;
	} else {
		cout << "Host-Error: Test Failed" << endl;	
	}

	// ============================================================================
	// Step 7: Custom Profiling
	// ============================================================================
	cout << "HOST-Info: ============================================================= " << endl;
	cout << "HOST-Info: (Step 7) Custom Profiling                                     " << endl;
	cout << "HOST-Info: ============================================================= " << endl;
	int Nb_Of_Kernels = Nb_Of_Exe_Events;
	int Nb_Of_Memory_Tranfers = Nb_Of_Mem_Events;

	string list_of_kernel_names[Nb_Of_Kernels]={"K_KVConstAdd","K_KpB","K_KA","K_KB","K_KCalc"};
	run_custom_profiling (Nb_Of_Kernels,Nb_Of_Memory_Tranfers,K_exe_event,Mem_op_event,list_of_kernel_names);

	// ============================================================================
	// Step 8: Release Allocated Resources
	// ============================================================================
	clReleaseDevice(Target_Device_ID); // Only available in OpenCL >= 1.2

	for (int i=0; i<Nb_Of_Mem_Events; i++) clReleaseEvent(Mem_op_event[i]);
	for (int i=0; i<Nb_Of_Exe_Events; i++) clReleaseEvent(K_exe_event[i]);

	clReleaseMemObject(GlobMem_BUF_DataIn_1);
	clReleaseMemObject(GlobMem_BUF_DataIn_2);
	clReleaseMemObject(GlobMem_BUF_DataIn_3);
	clReleaseMemObject(GlobMem_BUF_KpB);
	clReleaseMemObject(GlobMem_BUF_KA);
	clReleaseMemObject(GlobMem_BUF_KB);
	clReleaseMemObject(GlobMem_BUF_RES);

	clReleaseKernel(K_KVConstAdd);
	clReleaseKernel(K_KpB);
	clReleaseKernel(K_KA);
	clReleaseKernel(K_KB);
	clReleaseKernel(K_KCalc);

	clReleaseProgram(Program);
	clReleaseCommandQueue(Command_Queue);
	clReleaseContext(Context);

	free(Platform_IDs);
	free(Device_IDs);
	free(DataIn_1);
	free(DataIn_2);
	free(RES);

	cout << endl << "HOST-Info: DONE" << endl << endl;

	return EXIT_SUCCESS;
}

