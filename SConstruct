from os.path import join
import string

env = Environment(CCFLAGS="-std=c++0x")
env["prefix"] = "/usr/local"

def scanreplace(target, source, env):
	with open(str(source[0]), "r") as tmpl:
		result = string.Template(tmpl.read()).substitute(env)
	with open(str(target[0]), "w") as out:
		out.write(result)

env["BUILDERS"]["ScanReplace"] = Builder(action=scanreplace, src_suffix=".in")
env.ScanReplace("libahocorasick.pc", "libahocorasick.pc.in")

lib = env.SharedLibrary("ahocorasick", "ahocorasick.cpp")

installs = (("lib",lib), ("include", "ahocorasick.hpp"), 
	("lib/pkgconfig", "libahocorasick.pc"))
for (tgtdir,src) in installs:
	env.Alias("install", env.Install(join(env["prefix"], tgtdir), src))
