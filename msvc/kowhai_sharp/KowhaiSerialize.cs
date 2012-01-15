﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace kowhai_sharp
{
    public class KowhaiSerialize
    {
        [UnmanagedFunctionPointerAttribute(CallingConvention.Cdecl, CharSet=CharSet.Ansi)]
        public delegate string kowhai_get_symbol_name_t(UInt16 value);

        [DllImport(Kowhai.dllname, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern int kowhai_serialize(Kowhai.kowhai_tree_t tree, byte[] target_buffer, ref int target_size, kowhai_get_symbol_name_t get_name);

        [DllImport(Kowhai.dllname, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern int kowhai_deserialize(string buffer, byte[] scratch, int scratch_size, IntPtr descriptor, ref int descriptor_size, byte[] data, ref int data_size);

        public static int Serialize(Kowhai.kowhai_node_t[] descriptor, byte[] data, out string target, int targetBufferSize, kowhai_get_symbol_name_t getName)
        {
            byte[] targetBuf = new byte[targetBufferSize];
            Kowhai.kowhai_tree_t tree;
            GCHandle h = GCHandle.Alloc(descriptor, GCHandleType.Pinned);
            tree.desc = h.AddrOfPinnedObject();
            tree.data = data;
            int result = kowhai_serialize(tree, targetBuf, ref targetBufferSize, getName);
            h.Free();
            ASCIIEncoding enc = new ASCIIEncoding();
            target = enc.GetString(targetBuf, 0, targetBufferSize);
            return result;
        }

        public static int Deserialize(string buffer, out Kowhai.kowhai_node_t[] descriptor, out byte[] data)
        {
            int bufferSize = 0x100;

            int result;
            do
            {
                byte[] scratch = new byte[bufferSize];
                descriptor = new Kowhai.kowhai_node_t[bufferSize];
                data = new byte[bufferSize];
                int descriptorSize = bufferSize;
                int dataSize = bufferSize;

                GCHandle h = GCHandle.Alloc(descriptor, GCHandleType.Pinned);
                result = kowhai_deserialize(buffer, scratch, bufferSize, h.AddrOfPinnedObject(), ref descriptorSize, data, ref dataSize);
                h.Free();

                if (result == Kowhai.STATUS_OK)
                {
                    Array.Resize(ref descriptor, descriptorSize);
                    Array.Resize(ref data, dataSize);
                }

                bufferSize *= 2;
            }
            while (result == Kowhai.STATUS_SCRATCH_TOO_SMALL || result == Kowhai.STATUS_TARGET_BUFFER_TOO_SMALL);

            return result;
        }
    }
}
