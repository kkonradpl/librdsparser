librdsparser
=======

Parser library for Radio Data System with progressive text correction

Copyright (C) 2023-2024  Konrad Kosmatka

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

# RDS features 

Currently, the following RDS features are supported:

- Programme Identification (PI) code
- Programme Type (PTY) code with text lookup (RDS/RBDS)
- Traffic Programme (TP) flag
- Traffic Announcement (TA) flag
- Music/Speech (MS) flag
- Extended Country Code (ECC) with country lookup
- Alternative Frequency (AF)
- Programme Service (PS)
- Radio Text (RT)
- Programme Type Name (PTYN)
- Clock Time and Date (CT)

All the listed features are covered with unit and functional tests.

# Build
In order to build librdsparser you will need:

- CMake
- C compiler

Once you have all the necessary dependencies, you can use scripts available in the `build` directory.

Build options:
- `RDSPARSER_DISABLE_HEAP` - disable heap allocator, useful for embedded systems
- `RDSPARSER_DISABLE_UNICODE` - disable unicode support, useful to create a lightweight build

# Usage

Include the header:
```#include <librdsparser.h>```

For the heap allocator build (default) use:
```
rdsparser_t *rds = rdsparser_new();

if (rds == NULL)
{
	/* Unable to allocate the context */
	return -1;
}

/* When done, free up the memory */
rdsparser_free(rds);
```

For the stack allocation version, one needs to manually initialize the context:
```
rdsparser_t rds;
rdsparser_init(&rds);
```

# API overview

The library API is based on user callbacks that will handle the parsed data. The `rdsparser_register_*` functions provide a way to register own callbacks (only one function per callback type can be registered, use `NULL` function pointer to deregister the callback).

Each callback is triggered only when the data
- becomes available for the first time,
- changes its value.

To obtain current value in a callback use the corresponding `rdsparser_get_*` function. These functions can be also used outside a callback, as the data is internally buffered. It is also safe to use get functions for data that has not been received yet, as each data type provides a way to check whether the data is valid or not (unknown).

Use `rdsparser_parse(…)` to feed the parser with RDS data (four element `rdsparser_block` array, i.e. 4×16-bit) and information about error correction per block (four element `rdsparser_block_error` array, with `RDSPARSER_BLOCK_ERROR_*` values). If the decoder does not provide error correction, use `RDSPARSER_BLOCK_ERROR_NONE` for each block.

For the convenience of some existing protocols that use ASCII strings, there is also a function `rdsparser_parse_string(…)` that accepts hexadecimal string encoded data, like:

- ```A201200674697363``` (8 bytes, i.e. 4×16-bit blocks, with no information about error correction),

- ```34DD054AE3054F2015``` (8 bytes + 2 bytes with error correction levels).

The last two hex characters are encoded in the same way NXP provides the error correction level in the API of their tuners, i.e. the byte is encoded as follows:

- [7:6] - A block error correction level
- [5:4] - B block error correction level
- [3:2] - C block error correction level
- [1:0] - D block error correction level

where each 2-bit value is the `rdsparser_block_error` enum:

- 0 (`RDSPARSER_BLOCK_ERROR_NONE`) - no error found, no correction,
- 1 (`RDSPARSER_BLOCK_ERROR_SMALL`) - small error, data is corrected,
- 2 (`RDSPARSER_BLOCK_ERROR_LARGE`) - large error, data is corrected,
- 3 (`RDSPARSER_BLOCK_ERROR_UNCORRECTABLE`) - uncorrectable error or missing block; no data correction is possible (data will be discarded).

Use `rdsparser_clear(…)` to reset the data.

The up-to-date API usage example is available at `examples/main.c`. It is possible to use bindings for other programming languages. The JavaScript (Node.js) example is presented at `examples/nodejs/example.js`.

# Runtime configuration

Each callback provides the user-provided context in the last argument (`void *user_data`) which can be set using:

```
void rdsparser_set_user_data(rdsparser_t *rds, void *user_data)
```

Rarely, the RDS data is corrupted, despite being marked as valid and error correction free by the decoder. This is a limitation of the RDS itself. In order to make the decoding more robust (but at the cost of lower sensitivity and a latency of one occurrence), it is possible to use an extended check mode, where all periodically sent values are double checked (does not apply to strings or events that occur once, like CT).

```
void rdsparser_set_extended_check(rdsparser_t *rds, bool value)
bool rdsparser_get_extended_check(const rdsparser_t *rds)
```

For text strings there is a configurable maximum error correction level that will be used. By default, the parser uses only data that is marked as valid and not error-corrected in strings. The maximum level of character correction can be set for each text (PS, RT, PTYN) separately:

```
void rdsparser_set_text_correction(rdsparser_t *rds, rdsparser_text_t text, rdsparser_block_type_t type, rdsparser_block_error_t error)
rdsparser_block_error_t rdsparser_get_text_correction(const rdsparser_t *rds, rdsparser_text_t text, rdsparser_block_type_t type)
```

The progressive correction algorithm is disabled by default and can be enabled per text type using:

```
void rdsparser_set_text_progressive(rdsparser_t *rds, rdsparser_text_t string, bool state)
bool rdsparser_get_text_progressive(const rdsparser_t *rds, rdsparser_text_t string)
```

# Progressive correction

The unique feature of this library is an ability to build up text strings until they reach zero error correction levels. Strings sent within the RDS are generally static (or at least should be according to the specification). The algorithm is very straight-forward, i.e. each string character is replaced only with another that has the same weighted error correction level or lower. In the end, such string should be convergent to a correct one.

In order to use progressive correction, call both `rdsparser_set_text_correction` (set maximum error correction level) and `rdsparser_set_text_progressive` (enable the algorithm).

For example, when one wants to extract PS string characters, the following values are needed:
- Group type code (4 bits from the block B),
- Programme service name (2 bits from the block B),
- Programme service name segment (2 * 8 = 16 bits from the block D).

So, one needs only 4 + 2 = 6 bits from the second block B (`RDSPARSER_BLOCK_TYPE_INFO`) and all 16 bits from block D (`RDSPARSER_BLOCK_TYPE_DATA`). Because of that, a weighted method of calculating the overall error correction level of each character in a string has been implemented, where the data type block (C or D) has a higher weight than info block (B).

The progressive correction requires a decoder that is able to perform error correction and provide information about it, like NXP TEF668X.

# Projects using this library

- XDR-GTK (https://github.com/kkonradpl/xdr-gtk)
- FM-DX Webserver (https://github.com/NoobishSVK/fm-dx-webserver)
