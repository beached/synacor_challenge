// The MIT License (MIT)
//
// Copyright (c) 2014-2015 Darrell Wright
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files( the "Software" ), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cctype>

#include "memory_helper.h"
#include "helpers.h"
#include "vm.h"

int main( int argc, char** argv ) {
	virtual_machine_t vm;
	zero_fill( vm.registers );
	zero_fill( vm.memory );
	if( argc <= 1 ) {
		std::cerr << "Must supply a vm file" << std::endl;
		exit( EXIT_FAILURE );
	}
	vm.memory.from_file( argv[1] );

	std::cout << dump_memory( vm, [&]( size_t i ) {
		return  instructions::decoder( )[i];
	} ) << std::endl;

	return EXIT_SUCCESS;
}

