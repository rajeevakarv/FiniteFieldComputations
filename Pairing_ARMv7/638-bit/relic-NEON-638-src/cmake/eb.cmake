message(STATUS "Binary elliptic curve arithmetic configuration (EB module):\n")

message("   ** Options for the binary elliptic curve module (default = on, w = 4):")
message("      EB_ORDIN=[off|on] Support for ordinary curves.")
message("      EB_SUPER=[off|on] Support for supersingular curves.")    
message("      EB_KBLTZ=[off|on] Special support for Koblitz curves.")
message("      EB_STAND=[off|on] Support for standardized curves.")
message("      EB_MIXED=[off|on] Use mixed coordinates.")
message("      EB_PRECO=[off|on] Build precomputation table for generator.")
message("      EB_DEPTH=w        Width w in [2,6] of precomputation table for fixed point methods.")
message("      EB_WIDTH=w        Width w in [2,6] of window processing for unknown point methods.\n")

message("   ** Available binary elliptic curve methods (default = PROJC;WTNAF;COMBS;INTER):")
message("      EB_METHD=BASIC    Affine coordinates.")
message("      EB_METHD=PROJC    Projective coordinates (L�pez-Dahab for ordinary curves).\n")

message("      EB_METHD=BASIC    Binary method.")
message("      EB_METHD=CONST    Constant-time L�pez-Dahab point multiplication.")
message("      EB_METHD=WTNAF    Window (T)NAF method.\n")

message("      EB_METHD=BASIC    Binary method for fixed point multiplication.")
message("      EB_METHD=YAOWI    Yao's windowing method for fixed point multiplication")
message("      EB_METHD=NAFWI    NAF windowing method for fixed point multiplication.")
message("      EB_METHD=COMBS    Single-table Comb method for fixed point multiplication.")
message("      EB_METHD=COMBD    Double-table Comb method for fixed point multiplication.")
message("      EB_METHD=WTNAF    Window NAF with width w (TNAF for Koblitz curves).\n")

message("      EB_METHD=BASIC    Multiplication-and-addition simultaneous multiplication.")
message("      EB_METHD=TRICK    Shamir's trick for simultaneous multiplication.")
message("      EB_METHD=INTER    Interleaving of w-(T)NAFs.")
message("      EB_METHD=JOINT    Joint sparse form.\n")

message("      Note: these methods must be given in order. Ex: EB_METHD=\"BASIC;WTNAF;COMBD;TRICK\"\n")

if (NOT EB_DEPTH)
	set(EB_DEPTH 4)
endif(NOT EB_DEPTH)	
if (NOT EB_WIDTH)
	set(EB_WIDTH 4)
endif(NOT EB_WIDTH)	
set(EB_DEPTH "${EB_DEPTH}" CACHE STRING "Width of precomputation table for fixed point methods.")
set(EB_WIDTH "${EB_WIDTH}" CACHE STRING "Width of window processing for unknown point methods.")

option(EB_ORDIN "Support for ordinary curves" on)
option(EB_SUPER "Support for supersingular curves" on)
option(EB_KBLTZ "Special support for Koblitz curves" on)
option(EB_STAND "Support for NIST standardized curves" on)
option(EB_MIXED "Use mixed coordinates" on)
option(EB_PRECO "Build precomputation table for generator" on)

# Choose the arithmetic methods.
if (NOT EB_METHD)
	set(EB_METHD "PROJC;WTNAF;COMBS;INTER")
endif(NOT EB_METHD)
list(LENGTH EB_METHD EB_LEN)
if (EB_LEN LESS 4)
	message(FATAL_ERROR "Incomplete EB_METHD specification: ${EB_METHD}")
endif(EB_LEN LESS 4)

list(GET EB_METHD 0 EB_ADD)
list(GET EB_METHD 1 EB_MUL)
list(GET EB_METHD 2 EB_FIX)
list(GET EB_METHD 3 EB_SIM)
set(EB_METHD ${EB_METHD} CACHE STRING "Binary elliptic curve arithmetic method.")
