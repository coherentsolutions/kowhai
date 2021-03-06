﻿using System;
using System.Runtime.InteropServices;

namespace kowhai_sharp
{
    using uint8_t = Byte;
    using uint16_t = UInt16;
    using uint32_t = UInt32;
    using int16_t = Int16;
    using int32_t = Int32;

    public static class KowhaiProtocol
    {
        public const int CMD_GET_VERSION = 0x00;
        public const int CMD_GET_VERSION_ACK = 0x0F;
        public const int CMD_GET_TREE_LIST = 0x10;
        public const int CMD_GET_TREE_LIST_ACK = 0x1F;
        public const int CMD_GET_TREE_LIST_ACK_END = 0x1E;
        public const int CMD_WRITE_DATA = 0x20;
        public const int CMD_WRITE_DATA_END = 0x21;
        public const int CMD_WRITE_DATA_ACK = 0x2F;
        public const int CMD_READ_DATA = 0x30;
        public const int CMD_READ_DATA_ACK = 0x3F;
        public const int CMD_READ_DATA_ACK_END = 0x3E;
        public const int CMD_READ_DESCRIPTOR = 0x40;
        public const int CMD_READ_DESCRIPTOR_ACK = 0x4F;
        public const int CMD_READ_DESCRIPTOR_ACK_END = 0x4E;
        public const int CMD_GET_FUNCTION_LIST = 0x50;
        public const int CMD_GET_FUNCTION_LIST_ACK = 0x5F;
        public const int CMD_GET_FUNCTION_LIST_ACK_END = 0x5E;
        public const int CMD_GET_FUNCTION_DETAILS = 0x60;
        public const int CMD_GET_FUNCTION_DETAILS_ACK = 0x6F;
        public const int CMD_CALL_FUNCTION = 0x70;
        public const int CMD_CALL_FUNCTION_ACK = 0x7F;
        public const int CMD_CALL_FUNCTION_RESULT = 0x7E;
        public const int CMD_CALL_FUNCTION_RESULT_END = 0x7D;
        public const int CMD_CALL_FUNCTION_FAILED = 0x7C;
        public const int CMD_EVENT = 0x80;
        public const int CMD_EVENT_END = 0x8F;
        public const int CMD_GET_SYMBOL_LIST = 0x90;
        public const int CMD_GET_SYMBOL_LIST_ACK = 0x9F;
        public const int CMD_GET_SYMBOL_LIST_ACK_END = 0x9E;

        public const int CMD_ERROR_INVALID_COMMAND = 0xF0;
        public const int CMD_ERROR_INVALID_TREE_ID = 0xF1;
        public const int CMD_ERROR_INVALID_FUNCTION_ID = 0xF2;
        public const int CMD_ERROR_INVALID_SYMBOL_PATH = 0xF3;
        public const int CMD_ERROR_INVALID_PAYLOAD_OFFSET = 0xF4;
        public const int CMD_ERROR_INVALID_PAYLOAD_SIZE = 0xF5;
        public const int CMD_ERROR_INVALID_SEQUENCE = 0xF6;
        public const int CMD_ERROR_NO_DATA = 0xF7;
        public const int CMD_ERROR_UNKNOWN = 0xFF;

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct kowhai_protocol_header_t
        {
            public uint8_t command;
            public uint16_t id;
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct kowhai_protocol_symbol_spec_t
        {
            public uint8_t count;
            public IntPtr array_;
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct kowhai_protocol_data_payload_memory_spec_t
        {
            public uint16_t type;
            public uint16_t offset;
            public uint16_t size;
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct kowhai_protocol_data_payload_spec_t
        {
            public kowhai_protocol_symbol_spec_t symbols;
            public kowhai_protocol_data_payload_memory_spec_t memory;
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct kowhai_protocol_descriptor_payload_spec_t
        {
            public uint16_t node_count;
            public uint16_t offset;
            public uint16_t size;
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct kowhai_protocol_id_list_t
        {
            public uint16_t list_count;
            public uint16_t offset;
            public uint16_t size;
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct kowhai_protocol_id_list_item_t
        {
            public uint16_t id;
            public uint16_t type;
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct kowhai_protocol_string_list_t
        {
            public uint16_t list_count;
            public uint32_t list_total_size;
            public uint16_t offset;
            public uint16_t size;
        };

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct kowhai_protocol_function_details_t
        {
            public uint16_t tree_in_id;
            public uint16_t tree_out_id;
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct kowhai_protocol_function_call_t
        {
            public uint16_t offset;
            public uint16_t size;
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct kowhai_protocol_event_t
        {
            public uint16_t offset;
            public uint16_t size;
        }

        [StructLayout(LayoutKind.Explicit, Pack = 1)]
        public struct kowhai_protocol_payload_spec_t
        {
            [FieldOffset(0)]
            public uint32_t version;
            [FieldOffset(0)]
            public kowhai_protocol_id_list_t id_list;
            [FieldOffset(0)]
            public kowhai_protocol_data_payload_spec_t data;
            [FieldOffset(0)]
            public kowhai_protocol_descriptor_payload_spec_t descriptor;
            [FieldOffset(0)]
            public kowhai_protocol_function_details_t function_details;
            [FieldOffset(0)]
            public kowhai_protocol_function_call_t function_call;
            [FieldOffset(0)]
            public kowhai_protocol_function_call_t event_;
            [FieldOffset(0)]
            public kowhai_protocol_string_list_t string_list;
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct kowhai_protocol_payload_t
        {
            public kowhai_protocol_payload_spec_t spec;
            public IntPtr buffer;
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct kowhai_protocol_t
        {
            public kowhai_protocol_header_t header;
            public kowhai_protocol_payload_t payload;
        }

        [DllImport(Kowhai.dllname, CallingConvention = CallingConvention.Cdecl)]
        public static extern int kowhai_protocol_parse(IntPtr proto_packet, int packet_size, out kowhai_protocol_t protocol);

        [DllImport(Kowhai.dllname, CallingConvention = CallingConvention.Cdecl)]
        public static extern int kowhai_protocol_create(IntPtr proto_packet, int packet_size, ref kowhai_protocol_t protocol, out int bytes_required);

        [DllImport(Kowhai.dllname, CallingConvention = CallingConvention.Cdecl)]
        public static extern int kowhai_protocol_get_overhead(ref kowhai_protocol_t protocol, out int overhead);

        static void CopyIntPtrs(IntPtr dest, IntPtr source, int size)
        {
            // have to copy twice as Marshal.Copy has no IntPtr->IntPtr overload :((((
            uint8_t[] b = new uint8_t[size];
            Marshal.Copy(source, b, 0, size);
            Marshal.Copy(b, 0, dest, size);
        }

        public static int Parse(byte[] protoPacket, int packetSize, out kowhai_protocol_t protocol)
        {
            GCHandle h = GCHandle.Alloc(protoPacket, GCHandleType.Pinned);
            int result = kowhai_protocol_parse(h.AddrOfPinnedObject(), packetSize, out protocol);
            h.Free();
            return result;
        }

        public static int Parse(byte[] protoPacket, int packetSize, out kowhai_protocol_t protocol, out Kowhai.kowhai_symbol_t[] symbols)
        {
            GCHandle h = GCHandle.Alloc(protoPacket, GCHandleType.Pinned);
            int result = kowhai_protocol_parse(h.AddrOfPinnedObject(), packetSize, out protocol);
            h.Free();
            if (result == Kowhai.STATUS_OK && IsSymbolCommand(protocol.header.command))
            {
                symbols = new Kowhai.kowhai_symbol_t[protocol.payload.spec.data.symbols.count];
                h = GCHandle.Alloc(symbols, GCHandleType.Pinned);
                CopyIntPtrs(h.AddrOfPinnedObject(), protocol.payload.spec.data.symbols.array_, Marshal.SizeOf(typeof(Kowhai.kowhai_symbol_t)) * symbols.Length);
                h.Free();
            }
            else
                symbols = null;
            return result;
        }

        private static bool IsDescriptorCommand(uint8_t command)
        {
            if (command == KowhaiProtocol.CMD_READ_DESCRIPTOR)
                return true;
            if (command == KowhaiProtocol.CMD_READ_DESCRIPTOR_ACK)
                return true;
            if (command == KowhaiProtocol.CMD_READ_DESCRIPTOR_ACK_END)
                return true;
            return false;
        }

        private static bool IsSymbolCommand(uint8_t command)
        {
            switch (command)
            {
                case KowhaiProtocol.CMD_READ_DATA:
                case KowhaiProtocol.CMD_READ_DATA_ACK:
                case KowhaiProtocol.CMD_READ_DATA_ACK_END:
                case KowhaiProtocol.CMD_WRITE_DATA:
                case KowhaiProtocol.CMD_WRITE_DATA_ACK:
                    return true;
            }
            return false;
        }

        private static bool IsFunctionResultCommand(uint8_t command)
        {
            switch (command)
            {
                case KowhaiProtocol.CMD_CALL_FUNCTION_RESULT:
                case KowhaiProtocol.CMD_CALL_FUNCTION_RESULT_END:
                    return true;
            }
            return false;
        }

        private static bool IsEventCommand(uint8_t command)
        {
            switch (command)
            {
                case KowhaiProtocol.CMD_EVENT:
                case KowhaiProtocol.CMD_EVENT_END:
                    return true;
            }
            return false;
        }

        private static bool IsStringListResultCommand(uint8_t command)
        {
            switch (command)
            {
                case KowhaiProtocol.CMD_GET_SYMBOL_LIST_ACK:
                case KowhaiProtocol.CMD_GET_SYMBOL_LIST_ACK_END:
                    return true;
            }
            return false;
        }

        public static int CreateBasicPacket(byte[] protoPacket, int packetSize, ref kowhai_protocol_t protocol, out int bytesRequired)
        {
            GCHandle h = GCHandle.Alloc(protoPacket, GCHandleType.Pinned);
            int result = kowhai_protocol_create(h.AddrOfPinnedObject(), packetSize, ref protocol, out bytesRequired);
            h.Free();
            return result;
        }

        public static int CreateReadDataPacket(byte[] protoPacket, int packetSize, ref kowhai_protocol_t protocol, Kowhai.kowhai_symbol_t[] symbols, out int bytesRequired)
        {
            protocol.payload.spec.data.symbols.count = (uint8_t)symbols.Length;
            GCHandle h = GCHandle.Alloc(symbols, GCHandleType.Pinned);
            protocol.payload.spec.data.symbols.array_ = h.AddrOfPinnedObject();
            int result = CreateBasicPacket(protoPacket, packetSize, ref protocol, out bytesRequired);
            h.Free();
            return result;
        }

        public static int CreateWriteDataPacket(byte[] protoPacket, int packetSize, ref kowhai_protocol_t protocol, Kowhai.kowhai_symbol_t[] symbols, byte[] buffer, uint16_t offset, out int bytesRequired)
        {
            protocol.payload.spec.data.memory.size = (uint16_t)buffer.Length;
            protocol.payload.spec.data.memory.offset = offset;
            GCHandle h = GCHandle.Alloc(buffer, GCHandleType.Pinned);
            protocol.payload.buffer = h.AddrOfPinnedObject();
            int result = CreateReadDataPacket(protoPacket, packetSize, ref protocol, symbols, out bytesRequired);
            h.Free();
            return result;
        }

        public static int CreateCallFunctionPacket(byte[] protoPacket, int packetSize, ref kowhai_protocol_t protocol, byte[] buffer, uint16_t offset, out int bytesRequired)
        {
            protocol.payload.spec.function_call.offset = offset;
            if (buffer != null)
            {
                protocol.payload.spec.function_call.size = (uint16_t)buffer.Length;
                GCHandle h = GCHandle.Alloc(buffer, GCHandleType.Pinned);
                protocol.payload.buffer = h.AddrOfPinnedObject();
                int result = CreateBasicPacket(protoPacket, packetSize, ref protocol, out bytesRequired);
                h.Free();
                return result;
            }
            else
                protocol.payload.spec.function_call.size = 0;
            return CreateBasicPacket(protoPacket, packetSize, ref protocol, out bytesRequired);
        }

        public static void CopyDescriptor(Kowhai.kowhai_node_t[] target, kowhai_protocol_payload_t payload)
        {
            GCHandle h = GCHandle.Alloc(target, GCHandleType.Pinned);
            CopyIntPtrs(new IntPtr(h.AddrOfPinnedObject().ToInt64() + payload.spec.descriptor.offset), payload.buffer, payload.spec.descriptor.size);
            h.Free();
        }

        public static void CopyIdList(kowhai_protocol_id_list_item_t[] target, kowhai_protocol_payload_t payload)
        {
            GCHandle h = GCHandle.Alloc(target, GCHandleType.Pinned);
            CopyIntPtrs(new IntPtr(h.AddrOfPinnedObject().ToInt64() + payload.spec.id_list.offset), payload.buffer, payload.spec.id_list.size);
            h.Free();
        }

        public static uint8_t[] GetBuffer(kowhai_protocol_t prot)
        {
            byte[] buffer;
            if (IsDescriptorCommand(prot.header.command))
                buffer = new byte[prot.payload.spec.descriptor.size];
            else if (IsSymbolCommand(prot.header.command))
                buffer = new byte[prot.payload.spec.data.memory.size];
            else if (IsFunctionResultCommand(prot.header.command))
                buffer = new byte[prot.payload.spec.function_call.size];
            else if (IsEventCommand(prot.header.command))
                buffer = new byte[prot.payload.spec.event_.size];
            else if (IsStringListResultCommand(prot.header.command))
                buffer = new byte[prot.payload.spec.string_list.size];
            else
                return null;
            Marshal.Copy(prot.payload.buffer, buffer, 0, buffer.Length);
            return buffer;
        }
    }
}
