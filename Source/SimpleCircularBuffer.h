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

