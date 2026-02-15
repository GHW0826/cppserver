import argparse
import jinja2
import ProtoParser
from pathlib import Path
import shutil

def main():
	here = Path(__file__).resolve().parent
	root = here.parent
	default_proto = root / 'Core' / 'Protocol' / 'Protobuf' / 'Protocol.proto'
	default_dst_dir = root / 'Core' / 'Protocol'

	arg_parser = argparse.ArgumentParser(description = 'PacketGenerator')
	# arg_parser.add_argument('--path', type=str, default='D:/git/cppserver/Core/Protocol/Protobuf/Protocol.proto', help='proto path')
	arg_parser.add_argument('--path', type=str, default=default_proto, help='proto path')
	arg_parser.add_argument('--output', type=str, default='CorePacketHandler', help='output file')
	arg_parser.add_argument('--recv', type=str, default='C_', help='recv convention')
	arg_parser.add_argument('--send', type=str, default='S_', help='send convention')
	args = arg_parser.parse_args()

	parser = ProtoParser.ProtoParser(1000, args.recv, args.send)
	parser.parse_proto(args.path)
	file_loader = jinja2.FileSystemLoader('Templates')
	env = jinja2.Environment(loader=file_loader)

	template = env.get_template('PacketHandler.h')
	output = template.render(parser=parser, output=args.output)

	f = open(args.output+'.h', 'w+')
	f.write(output)
	f.close()


	# Move To Core Lib Folder
	src = Path(r"CorePacketHandler.h")
	dst_dir = Path(default_dst_dir)
	# dst_dir = Path(r"D:\git\cppserver\Core\Protocol")
	dst_dir.mkdir(parents=True, exist_ok=True)
	shutil.move(str(src), str(dst_dir / src.name))
	# print(output)
	return

if __name__ == '__main__':
	main()