/*
 The MIT License (MIT)

 Copyright (c) 2016 Martin Braun

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

//compile with: javac Main.java
//run with java Main

import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;
import java.util.stream.Collectors;

/**
 * @author Martin Braun
 */
public class Main {

	private static final int[] sequence = {9, 2, 8, 7, 6, 5, 4, 3, 1};
	private static final int[][] successors = {
			{}, //0
			{2}, //1
			{3, 9}, //2
			{4, 5}, //3
			{8}, //4
			{6, 7}, //5
			{8}, //6
			{8}, //7
			{2}, //8
			{} //9
	};

	public static void main(String[] args) {
		@SuppressWarnings("unchecked")
		Set<String>[] use = (Set<String>[]) new Set[] {
				set(), //0
				set(), //1
				set( "i", "n" ), //2
				set(), //3
				set( "reset" ), //4
				set( "i" ), //5
				set( "j" ), //6
				set( "sum", "j" ), //7
				set( "i" ), //8
				set( "sum", "reset" ) //9
		};

		@SuppressWarnings("unchecked")
		Set<String>[] def = (Set<String>[]) new Set[] {
				set(), //0
				set( "i", "n" ), //1
				set(), //2
				set( "t1", "j" ), //3
				set( "sum" ), //4
				set(), //5
				set( "sum" ), //6
				set(), //7
				set(), //8
				set() //9
		};

		Set<String>[] in = empty( 10 );
		Set<String>[] out = empty( 10 );

		Set<String>[] newIn = copy( in );
		Set<String>[] newOut = copy( out );

		do {
			in = newIn;
			out = newOut;
			newIn = copy( in );
			newOut = copy( out );
			for ( int B : sequence ) {
				System.out.println("current B" + B + ":");

				for ( int succ : successors[B] ) {
					System.out.println("current successor: " + succ + ":");
					newOut[B].addAll( newIn[succ] );
					System.out.println("out[" + B + "]=" + newOut[B]);
				}

				newIn[B].clear();
				newIn[B].addAll( use[B] );
				newIn[B].addAll( newOut[B].stream()
										 .filter( elem -> !def[B].contains( elem ) )
										 .collect( Collectors.toList() ) );
				System.out.println("in[" + B + "]=" + newIn[B]);

				System.out.println("B after going through successors:");
				System.out.println("in[" + B + "]=" + newIn[B]);
				System.out.println("out[" + B + "]=" + newOut[B]);

				System.out.println("========================================");
			}
		} while ( !Arrays.equals( in, newIn ) || !Arrays.equals( out, newOut ) );

		System.out.println("Final Output:");
		for(int B = 0; B < in.length; ++B) {
			System.out.println("in[" + B + "]=" + in[B]);
		}
		for(int B = 0; B < out.length; ++B) {
			System.out.println("out[" + B + "]=" + out[B]);
		}
	}

	public static Set<String> set(String... strs) {
		return new HashSet<>( Arrays.asList( strs ) );
	}

	public static Set<String>[] empty(int size) {
		@SuppressWarnings("unchecked")
		Set<String>[] ret = new Set[size];
		for ( int i = 0; i < ret.length; ++i ) {
			ret[i] = new HashSet<>();
		}
		return ret;
	}

	public static Set<String>[] copy(Set<String>[] sets) {
		Set<String>[] ret = empty( sets.length );
		for ( int i = 0; i < sets.length; ++i ) {
			ret[i].addAll( sets[i] );
		}
		return ret;
	}

}
