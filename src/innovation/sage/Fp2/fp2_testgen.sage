
##*************************************************************************************/
##/* Copyright (C) 2022 - Renaud Dubois - This file is part of cy_lib project	 */
##/* License: This software is licensed under a dual BSD and GPL v2 license. 	 */
##/* See LICENSE file at the root folder of the project.				 */
##/* FILE: test_musig2.sage							             	  */
##/* 											  */
##/* 											  */
##/* DESCRIPTION: Sage module for quadratic extensions validation		  */
##**************************************************************************************/


load('../common/io_conversions.sage');

#write a variable in a C like style with prefix and suffix comments into a file
def fprint_fp2_MSB(f, prefix, name, A, B, size_word, setlength, suffix):
	
	concat_str=prefix;
	sizeA=ceil(log(A)/log(2))  
	sizeM=ceil(sizeA/size_word)
	
	concat_str=concat_str+"\n uint8_t "+ name+ "[2]["+str( sizeM)+"]={{";
	M=Conv_word_MSB(A, size_word);
	for i in [0..sizeM-2]:
		concat_str=concat_str+" "+hex(M[i])+",";
	concat_str=concat_str+" "+hex(M[sizeM-1])+"},{";
	M=Conv_word_MSB(B, size_word);
	for i in [0..sizeM-2]:
		concat_str=concat_str+" "+hex(M[i])+",";
	
	
	
	
	concat_str=concat_str+suffix;
	
	f.write(concat_str);
	
	return concat_str;
	
def init_FP2(p):
	K = GF(p)
	R.<t> = PolynomialRing(K)
	Fp2=K.extension(t^2 + 1, 'i')
	return Fp2;


p_bls381=0x1a0111ea397fe69a4b1ba7b6434bacd764774b84f38512bf6730d2a0f6b0f6241eabfffeb153ffffb9feffffffffaaab;
p_bls377=0x01ae3a4617c510eac63b05c06ca1493b1a22d9f300f5138f1ef3622fba094800170b5d44300000008508c00000000001;


