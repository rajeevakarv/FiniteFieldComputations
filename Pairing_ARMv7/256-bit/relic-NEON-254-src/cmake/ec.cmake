message(STATUS "Elliptic curve cryptography configuration (EC module):\n")

message("   ** Options for the binary elliptic curve module (default = on):")
message("      EC_KBLTZ=[off|on] Prefer Koblitz (prime or binary) curves.\n")

message("   ** Available elliptic curve methods (default = PRIME):")
message("      EC_METHD=PRIME    Use prime curves.")
message("      EC_METHD=BINAR    Use binary curves.\n")

message("      Note: these methods must be given in order. Ex: EC_METHD=\"BINAR\"\n")

option(EC_KBLTZ "Prefer Koblitz curves" on)

# Choose the arithmetic methods.
if (NOT EC_METHD)
	set(EC_METHD "PRIME")
endif(NOT EC_METHD)
list(LENGTH EC_METHD EC_LEN)
if (EC_LEN LESS 1)
	message(FATAL_ERROR "Incomplete EC_METHD specification: ${EC_METHD}")
endif(EC_LEN LESS 1)

list(GET EC_METHD 0 EC_CUR)
set(EC_METHD ${EC_METHD} CACHE STRING "Choice of elliptic curve.")
