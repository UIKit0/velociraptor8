#!/usr/bin/env python

import os, subprocess, shutil

g_src_dir = os.path.realpath(os.path.join("..", "sumatrapdf", "src", "utils"))
assert os.path.exists(g_src_dir), "%s doesn't exist" % g_src_dir
g_dst_dir = os.path.realpath(os.path.join("src", "utils"))
print("src dir: %s, dst dir: %s" % (g_src_dir, g_dst_dir))
if not os.path.exists(g_dst_dir):
	os.mkdir(g_dst_dir)

os.chdir(g_src_dir)
subprocess.call(["svn", "update"])

files = os.listdir(g_src_dir)

blacklist = ["ArchUtil", "BitReader", "ByteOrderDecoder", "ByteWriter", "CryptoUtil", "CssParser", "DialogSizer", "Dict", "DirIter", "FileTransactions", "HtmlParserLookup", "HtmlPrettyPrint", "HtmlPullParser", "JsonParser", "LzmaSimpleArchive", "NoFreeAllocator", "PalmDbReader", "SerializeTxt", "SettingsUtil", "SimpleLog", "SquareTreeParser", "TrivialHtmlParser", "TxtParser", "VarintGob", "ZipUtil"]

def should_copy(f):
	if not (f.endswith(".h") or f.endswith(".cpp")):
		return False
	if "_ut." in f:
		return False
	for s in blacklist:
		if s in f:
			return False
	return True

files = [f for f in files if should_copy(f)]
for f in files:
	src = os.path.join(g_src_dir, f)
	dst = os.path.join(g_dst_dir, f)
	shutil.copy(src, dst)
	print("copied %s to %s" % (src, dst))
