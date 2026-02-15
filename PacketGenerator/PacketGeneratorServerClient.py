import argparse
import jinja2
import ProtoParser
from pathlib import Path
import shutil

def main():
	here = Path(__file__).resolve().parent
	root = here.parent

	default_proto = root / 'Common' / 'Protocol' / 'Protobuf' / 'CSProtocol.proto'
	client_dst_dir = root / 'Test' / 'Protocol'
	server_dst_dir = root / 'Server' / 'Protocol'

	#### Client Packet Handler
	client_arg_parser = argparse.ArgumentParser(description = 'PacketGenerator')
	client_arg_parser.add_argument('--path', type=str, default=default_proto, help='proto path')
	client_arg_parser.add_argument('--output', type=str, default='ServerPacketHandler', help='output file')
	client_arg_parser.add_argument('--recv', type=str, default='S_', help='recv convention')
	client_arg_parser.add_argument('--send', type=str, default='C_', help='send convention')
	client_args = client_arg_parser.parse_args()

	client_parser = ProtoParser.ProtoParser(1000, client_args.recv, client_args.send)
	client_parser.parse_proto(client_args.path)
	client_file_loader = jinja2.FileSystemLoader('Templates')
	client_env = jinja2.Environment(loader=client_file_loader)

	client_template = client_env.get_template('ServerPacketHandler.h')
	client_output = client_template.render(parser=client_parser, output=client_args.output)

	client_f = open(client_args.output+'.h', 'w+')
	client_f.write(client_output)
	client_f.close()

	# Move To Core Lib Folder
	client_src = Path(r"ServerPacketHandler.h")
	client_dst_dir = Path(client_dst_dir)
	client_dst_dir.mkdir(parents=True, exist_ok=True)
	shutil.move(str(client_src), str(client_dst_dir / client_src.name))


	#### Server Packet Handler
	server_arg_parser = argparse.ArgumentParser(description = 'PacketGenerator')
	server_arg_parser.add_argument('--path', type=str, default=default_proto, help='proto path')
	server_arg_parser.add_argument('--output', type=str, default='ClientPacketHandler', help='output file')
	server_arg_parser.add_argument('--recv', type=str, default='C_', help='recv convention')
	server_arg_parser.add_argument('--send', type=str, default='S_', help='send convention')
	server_args = server_arg_parser.parse_args()

	server_parser = ProtoParser.ProtoParser(1000, server_args.recv, server_args.send)
	server_parser.parse_proto(server_args.path)
	server_file_loader = jinja2.FileSystemLoader('Templates')
	server_env = jinja2.Environment(loader=server_file_loader)

	server_template = server_env.get_template('ClientPacketHandler.h')
	server_output = server_template.render(parser=server_parser, output=server_args.output)

	server_f = open(server_args.output+'.h', 'w+')
	server_f.write(server_output)
	server_f.close()

	# Move To Core Lib Folder
	server_src = Path(r"ClientPacketHandler.h")
	server_dst_dir = Path(server_dst_dir)
	server_dst_dir.mkdir(parents=True, exist_ok=True)
	shutil.move(str(server_src), str(server_dst_dir / server_src.name))

	return

if __name__ == '__main__':
	main()