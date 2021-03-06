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

#include <fstream>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include "vm.h"
#include "vm_control.h"
#include "helpers.h"

namespace {
	std::string dump_regs( virtual_machine_t & vm ) {
		std::stringstream ss;
		for( size_t n = 0; n < vm.registers.size( ); ++n ) {
			ss << "REG" << n << ": " << vm.registers[n] << "\n";
		}
		ss << "Instruction ptr: " << vm.instruction_ptr << "\n";
		return ss.str( );
	}

}

namespace {
	void save_to_text_file( boost::string_ref fname, boost::string_ref text ) {
		std::ofstream fout;
		fout.open( fname.data( ) );
		if( !fout ) {
			std::cerr << "Error saving to " << fname << "\n";
			exit( EXIT_FAILURE );
		}
		fout << text;
		fout.close( );		
	}

}

void vm_control::show_asm( virtual_machine_t& vm, std::vector<uint16_t> const& tokens ) {
	uint16_t from_address = 0;
	uint16_t to_address = std::numeric_limits<uint16_t>::max( );
	if( tokens.size( ) > 0 ) {
		from_address = tokens[0];
	}
	if( tokens.size( ) > 1 ) {
		to_address = tokens[1];
	}

	full_dump( vm, from_address, to_address );
	std::cout << "\n\n";
}


void vm_control::save_asm( virtual_machine_t & vm, boost::string_ref fname ) {
	save_to_text_file( fname, full_dump_string( vm ) );
	std::cout << "Saved file to " << fname << "\n";
}

void vm_control::get_ip( virtual_machine_t & vm ) {
	std::cout << "Current instruction ptr is " << vm.instruction_ptr << "\n";
}

void vm_control::tick( virtual_machine_t & vm ) {
	vm.tick( true );
	get_ip( vm );
}

void vm_control::get_regs( virtual_machine_t & vm ) {
	std::cout << "Current register values\n";
	std::cout << dump_regs( vm ) << "\n";
}

void vm_control::get_bps( virtual_machine_t & vm ) {
	std::cout << "Current breakpoints(" << vm.debugging.breakpoints.size( ) << ")\n";
	for( auto const & bp : vm.debugging.breakpoints ) {
		std::cout << bp << "\n";
	}
}

void vm_control::clear_bps( virtual_machine_t & vm ) {
	std::cout << "Clearing " << vm.debugging.breakpoints.size( ) << " breakpoints\n";
	vm.debugging.breakpoints.clear( );
}

void vm_control::get_memory_traps(virtual_machine_t& vm) {
	std::cout << "Current memory traps(" << vm.debugging.memory_traps.size( ) << ")\n";
	for( auto const & bp : vm.debugging.memory_traps ) {
		std::cout << bp << "\n";
	}
}

void vm_control::clear_memory_traps( virtual_machine_t& vm ) {
	std::cout << "Clearing " << vm.debugging.memory_traps.size( ) << " memory traps\n";
	vm.debugging.memory_traps.clear( );
}

void vm_control::save_state( virtual_machine_t & vm, boost::string_ref fname ) {
	vm.save_state( fname );
	std::cout << "State saved to file '" << fname << "'\n";
}

void vm_control::load_state( virtual_machine_t & vm, boost::string_ref fname ) {
	vm.load_state( fname );
	std::cout << "Loaded state from file '" << fname << "'\n";
}

void vm_control::show_argument_stack( virtual_machine_t & vm ) {
	std::cout << "Current argument stack(" << vm.argument_stack.size( ) << ")\n";
	for( size_t n = 0; n < vm.argument_stack.size( ); ++n ) {
		std::cout << n << ": " << vm.argument_stack[n] << "\n";
	}
}

void vm_control::show_program_stack( virtual_machine_t & vm ) {
	std::cout << "Current program stack(" << vm.program_stack.size( ) << ")\n";
	for( size_t n = 0; n < vm.program_stack.size( ); ++n ) {
		std::cout << n << ": " << vm.program_stack[n] << "\n";
	}
}

void vm_control::save_trace(virtual_machine_t& vm, boost::string_ref fname) {
	save_to_text_file( fname, vm.debugging.trace.to_json( ) );
	boost::filesystem::rename( "trace_state.bin", fname.to_string( ) + ".state" );
}

void vm_control::start_tracing( virtual_machine_t & vm ) {
	save_state( vm, "trace_state.bin" );
	vm.debugging.enable_tracing = true;
}

void vm_control::stop_tracing( virtual_machine_t & vm ) {
	vm.debugging.enable_tracing = false;
}
