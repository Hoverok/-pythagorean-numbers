//this university project was meant as an introduction into SSE instructions for xmm registers in assembler
//the program checks what pairs of numers (under 1000) are pythagorean numbers (c^2 = a^2 + b^2)
//an xmm register fits 4 float numbers
//the program feeds numbers into the registers and goes through all possible pairs
//example: xmm0= [1,2,3,4] xmm1= [2,3,4,5], next loop is xmm0= [1,2,3,4] xmm1=[2,3,4,5] until xmm1 is [997,998,999,1000]
//then xmm0 changes to [2,3,4,5] and xmm1 is reset to xmm0 (+1) [3,4,5,6] until xmm1 reaches 1000
//the assembler function squares each number in the register, adds it to the square of the other register number's square,
//creates a copy of that sum, rounds it and then substarcts the rounded value from the original sum of squares
//if the result is equal to 0, the pair are pythagorean numbers

#include <iostream>
#include <emmintrin.h>

using namespace std;
//xmm registras yra 128 bitu ir siuo atveju laiko 4 x 32bitu floatus


void fill_R1(float R1[]) {
	for (int i = 0; i < 1000; i++) {
		R1[i] = i + 1;
	}
}




// sqrtps xmm0, xmm1 
//Computes Square Roots of the packed single-precision floating-point values in xmm1/m128 and stores the result in xmm0.

int main() {
	_declspec(align(16))float R1[1000];
	_declspec(align(16))float R2[4]; // is jo skaitis skaicius i xmm1
	_declspec(align(16))float M[24];



	float* ptr1;
	float* ptr2;
	float* ptr3;
	float* ptr4;
	float* ptr5;
	ptr2 = R2; //laikinas masyvas
	//blokas- 4 floatai (xmm registras)
	ptr3 = M;  //rezu 1 blokas
	ptr4 = ptr3 + 4; //rezu 2 blokas
	ptr5 = ptr4 + 4; //rezu 3 blokas

	fill_R1(R1);


	for (int i = 0; i < 1000; i += 4) { //paskutinis i = 996
		for (int j = i+1; j < 1000; j ++){ //paskutinis j = 997
		ptr1 = R1+i;  //rodo i current R1 elemento vieta adrese
		//R2 yra aligned masyvas, i ji antru ciklu bus uzrasomi 4 skaiciai
		//skaiciai visada prasides nuo 1 daugiau, nes j=i+1
		//pvz R1 1,2,3,4, tai R2 prasides 2,3,4,5(3,4,5,6 ir taip iki 1000-1003)
		R2[0] = R1[j];      
		R2[1] = R1[j] + 1;
		R2[2] = R1[j] + 2;
		R2[3] = R1[j] + 3;
		//nes reikia patikrini nuo 1 iki 1000


		_asm {
			mov eax, ptr1 // R1 masyvas
			mov ecx, ptr2 // R2 masyvas
			movaps xmm0, [eax] // i xmm0 iraso 4 R1 elementus
			movaps xmm1, [ecx] // i xmm1 4 R2 elemtus
			movaps xmm2, xmm0   // daro kopija pradiniu skaiciu
			movaps xmm3, xmm1   // daro kopija pradiniu skaiciu
								// isvesiu pradinius skaicius ir galutini palyginimo rezultatus i M
			mulps xmm2, xmm2 //kelia kvadratu
			mulps xmm3, xmm3 //kelia kvadratu
			addps xmm2, xmm3  //kvadratu suma
			sqrtps xmm3, xmm2 // uzraso xmm2 sakni i xmm3
			movaps xmm4, xmm3 //!!!!!!!! daro kvadratu sumos saknio kopija
			roundps xmm4, xmm4, 1 //apvalyna zemyn
			mulps xmm4, xmm4  // kelia apvalinta skaiciu kvadratu
			subps xmm2, xmm4  // original kvadratu suma minus apvalinta

			mov eax, ptr3
			mov ebx, ptr4
			mov ecx, ptr5

			movaps[eax], xmm0 //original R1 skaiciai
			movaps[ebx], xmm1 //original R2 skaiciai
			movaps[ecx], xmm2 //kvadratu skirtumas (jei 0, pit skaiciai)

		}
		// M[0-3] pirmas blokas, M[4-7] antras blokas, M[8-11] kvadratu paliginimo blokas
		//nes reikia patikrini nuo 1 iki 1000
		//R[0] == 1000 is ok
			 if (M[8] == 0) cout << M[0] << " ir " << M[4] << " yra Pit skaiciai!\n";
			 if (M[9] == 0 && M[5] <= 1000) cout << M[1] << " ir " << M[5] << " yra Pit skaiciai!\n";
			 if (M[10] == 0 && M[6] <= 1000) cout << M[2] << " ir " << M[6] << " yra Pit skaiciai!\n";
			 if (M[11] == 0 && M[7] <= 1000) cout << M[3] << " ir " << M[7] << " yra Pit skaiciai!\n";


	}
	}
}