using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace MPU6050_SerialReceiver_DynamicLibrary_Demo_in_Sharp
{
    class Program
    {
        [UnmanagedFunctionPointerAttribute(CallingConvention.Cdecl)]

        public delegate int ProcCallBack(char direction, float pitch, float roll, float yaw);

        [DllImport("MPU6050-SerialReceiver-DynamicLibrary.dll", EntryPoint = "fnMPU6050SerialReceiverDynamicLibrary", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]

        public static extern int fnMPU6050SerialReceiverDynamicLibrary(string COMx, Int32 BaudRate, ProcCallBack ProcessCalslBackFunc);

        static int ProcessCallBackFuncInstance(char direction, float pitch, float roll, float yaw)
        {
            // For exmaple: 
            if (direction == 1)
            {
                Console.WriteLine("Left pitch:{0} roll:{1} yaw:{2}", pitch, roll, yaw);
            }
            else if (direction == 2)
            {
                Console.WriteLine("Right pitch:{0} roll:{1} yaw:{2}", pitch, roll, yaw);
            }
            // eg end.

            // Do anything you want.
            // ...

            return 0;
        }

        static void Main(string[] args)
        {
            Int32 ret = fnMPU6050SerialReceiverDynamicLibrary("COM4", 115200, new ProcCallBack(ProcessCallBackFuncInstance));

            while (true);
        }
    }
}
