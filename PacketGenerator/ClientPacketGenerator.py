import argparse
import jinja2
import ProtoParser
from pathlib import Path
import shutil

def main():
	arg_parser = argparse.ArgumentParser(description = 'PacketGenerator')
	arg_parser.add_argument('--path', type=str, default='D:/git/cppserver/Common/Protocol/Protobuf/CSProtocol.proto', help='proto path')
	arg_parser.add_argument('--output', type=str, default='ClientPacketHandler', help='output file')
	arg_parser.add_argument('--recv', type=str, default='C_', help='recv convention')
	arg_parser.add_argument('--send', type=str, default='S_', help='send convention')
	args = arg_parser.parse_args()

	parser = ProtoParser.ProtoParser(1000, args.recv, args.send)
	parser.parse_proto(args.path)
	file_loader = jinja2.FileSystemLoader('Templates')
	env = jinja2.Environment(loader=file_loader)

	template = env.get_template('ClientPacketHandler.h')
	output = template.render(parser=parser, output=args.output)

	f = open(args.output+'.h', 'w+')
	f.write(output)
	f.close()

	# Move To Core Lib Folder
	src = Path(r".\ClientPacketHandler.h")
	dst_dir = Path(r"D:\git\cppserver\Server\Protocol")
	dst_dir.mkdir(parents=True, exist_ok=True)
	shutil.move(str(src), str(dst_dir / src.name))
	# print(output)
	return

if __name__ == '__main__':
	main()