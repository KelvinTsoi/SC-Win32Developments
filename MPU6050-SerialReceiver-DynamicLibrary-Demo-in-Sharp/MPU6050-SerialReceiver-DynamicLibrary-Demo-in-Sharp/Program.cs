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

        [DllImport("MPU6050-SerialReceiver-DynamicLibrary.dll", EntryPoint = "fnMPU6050SerialReceiverDynamicLibraryProcess", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]

        public static extern int fnMPU6050SerialReceiverDynamicLibraryProcess(string COMx, Int32 BaudRate, ProcCallBack ProcessCalslBackFunc);

        [DllImport("MPU6050-SerialReceiver-DynamicLibrary.dll", EntryPoint = "fnMPU6050SerialReceiverDynamicLibraryKillProcess", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]

        public static extern int fnMPU6050SerialReceiverDynamicLibraryKillProcess();

        delegate bool ConsoleCtrlDelegate(int dwCtrlType);

        [DllImport("kernel32.dll")]
        private static extern bool SetConsoleCtrlHandler(ConsoleCtrlDelegate HandlerRoutine, bool Add);

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
            Int32 ret = 0;

            if((ret=fnMPU6050SerialReceiverDynamicLibraryProcess("COM4", 115200, new ProcCallBack(ProcessCallBackFuncInstance)))!=0)
            {
                Console.WriteLine("Start Process Error, Error Code:{0}", ret);
                return;
            }

            ConsoleCtrlDelegate newDelegate = new ConsoleCtrlDelegate(HandlerRoutine);

            if (SetConsoleCtrlHandler(newDelegate, true))
            {
                while (true) ;
            }
            return;
        }

        const int CTRL_C_EVENT = 0;

        const int CTRL_BREAK_EVENT = 1;

        static bool HandlerRoutine(int CtrlType)
        {
            Int32 ret = 0;
            switch (CtrlType)
            {
                case CTRL_C_EVENT:
                    if ((ret = fnMPU6050SerialReceiverDynamicLibraryKillProcess()) != 0)
                    {
                        Console.WriteLine("Start Process Error, Error Code:{0}", ret);
                        return false;
                    }
                    break;
                case CTRL_BREAK_EVENT:
                    if ((ret = fnMPU6050SerialReceiverDynamicLibraryProcess("COM4", 115200, new ProcCallBack(ProcessCallBackFuncInstance))) != 0)
                    {
                        Console.WriteLine("Stop Process Error, Error Code:{0}", ret);
                        return false;
                    }
                    break;
                default:
                    return true;
            }
            return true;
        }
    }
}
