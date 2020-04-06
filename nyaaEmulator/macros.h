#pragma once

#ifdef _DEBUG
#define Debug(x) std::cout << x
#else
#define Debug(x) 
#endif

#define HEX(x) std::hex << (x) << std::dec

#define BIN(x) std::dec << ((int)x) << std::dec
