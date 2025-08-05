/*
  ==============================================================================

	SimpleCircularBuffer.h
	Created: 3 Mar 2025 2:15:41am
	Author:  jorda

	This implementation has been retooled to allow for buffers of non power of two sizes
	AND to access elements from newest to oldest.

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

template <size_t SIZE>
class SimpleCircularBuffer {
public:

	SimpleCircularBuffer() = default;
	~SimpleCircularBuffer() = default;

	void push(float element) {
		buffer[index] = element;
		index = (index + 1) % (SIZE);
	}

	//Pop the last element and push a new one, specified by the user
	float pop(float element) {
		float temp = operator[](0);
		push(element);
		return temp;
	}

	//Access from newest to oldest
	float& operator[](size_t x) {
		return buffer[(index + SIZE - 1 - x) % (SIZE)];
	}
	float operator[](size_t x) const {
		return buffer[(index + SIZE - 1 - x) % (SIZE)];
	}

	float* getArrayData() {
		return buffer.data();
	}

	int getSize() {
		return SIZE;
	}

private:
	std::array<float, SIZE> buffer{ 0.f };
	size_t index = 0;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleCircularBuffer)
};

//[A, B, C, D, E, F, G, H, I, J]
//push A through D, assume array SIZE = 8
//vector looks like [A, B, C, D, 0, 0, 0, 0], but visual representation should be [0, 0, 0, 0, A, B, C, D]
//index = 4
//We want [0] to return 0, [4] to return A
//We return vector[(index + x) % size] for operator[]
//Push A through E, SIZE = 8
//Vector looks like [A, B, C, D, E, 0, 0, 0], vr is [0, 0, 0, A, B, C, D, E]
//index = 5
//We want [0] to return 0, [3] to return A
//We return vector[(index + x) % size] for operator[]
//Push A through J, SIZE = 8
//Vector looks like [I, J, C, D, E, F, G, H], vr is [C, D, E, F, G, H, I, J]
//index = 2
//We want [0] to return C, [6] to return I, [7] to return J
//We return vector[(index + x) % size] for operator[]