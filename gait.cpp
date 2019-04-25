#include "gait.h"
#include "config.h"
#include <stdlib.h>

Gait::Gait(uint8 _id)
{
	id = _id;
	periods = NULL;
	amplitudes = NULL;
	offsets = NULL;
	phases = NULL;
	type = GaitType::IDLE;
	iterations = 0;
}

Gait::~Gait()
{
	if (offsets)
		free(offsets);
	if (phases)
		free(phases);
	if (periods)
		free(periods);
	if (amplitudes)
		free(amplitudes);
	periods = NULL;
	amplitudes = NULL;
	offsets = NULL;
	phases = NULL;
}

void Gait::setId(uint8 _id)
{
	if (id)
		return;

	id = _id;
}

/**
 * @brief Gait::setup Init gait memory
 */
void Gait::setup()
{
	size_t memory_size = (size_t)(getTotalCount() / 2);

	periods = (uint16*)malloc(sizeof (uint16) * memory_size);
	amplitudes = (int8*)malloc(sizeof (int8) * memory_size);
	offsets = amplitudes + memory_size;
	phases = periods + memory_size;

	for (uint8 i = 0; i < memory_size; i++)
	{
		periods[i] = GAIT_DEFAULT_PERIOD;	//1sec
		amplitudes[i] = GAIT_DEFAULT_AMPLITUDE;
		offsets[i] = 0;
		phases[i] = 0;
	}
	duration = GAIT_DEFAULT_DURATION;
}

// ---- Duration
void Gait::updateDuration()
{
	duration = 0;
	for (uint16 i = 0; i < getTotalCount(); i++)
	{
		if (periods[i] > duration)
			duration = periods[i];
	}
}

// ---- Period
void Gait::setPeriods(uint16 *_periods)
{
	for (uint16 i = 0; i < getTotalCount(); i++)
		periods[i] = _periods[i];
	updateDuration();
}

void Gait::setPeriod(int index, uint16 period)
{
	periods[index] = period;
	updateDuration();
}

void Gait::fillPeriods(uint16 period)
{
	for (uint16 i = 0; i < getTotalCount(); i++)
		periods[i] = period;
	duration = period;
}

uint16 Gait::getPeriod(uint8 index)
{
	if (periods)
		return periods[index];
	return GAIT_DEFAULT_PERIOD;
}

// ---- Amplitude
void Gait::setAmplitudes(int8 *_amplitudes)
{
	for (uint16 i = 0; i < getTotalCount(); i++)
		amplitudes[i] = _amplitudes[i];
}

void Gait::setAmplitude(int index, int8 amplitude)
{
	amplitudes[index] = amplitude;
}

void Gait::fillAmplitudes(int8 amplitude)
{
	for (uint16 i = 0; i < getTotalCount(); i++)
		amplitudes[i] = amplitude;
}

int8 Gait::getAmplitude(uint8 index)
{
	if (amplitudes)
		return amplitudes[index];
	return GAIT_DEFAULT_AMPLITUDE;
}

// ---- Offset
void Gait::setOffsets(int8 *_offsets)
{
	for (uint16 i = 0; i < getTotalCount(); i++)
		offsets[i] = _offsets[i];
}

void Gait::setOffset(int index, int8 offset)
{
	offsets[index] = offset;
}

void Gait::fillOffsets(int8 offset)
{
	for (uint16 i = 0; i < getTotalCount(); i++)
		offsets[i] = offset;
}

int8 Gait::getOffset(uint8 index)
{
	if (offsets)
		return offsets[index];
	return 0;
}

// ---- Phase
void Gait::setPhases(uint16 *_phases)
{
	for (uint16 i = 0; i < getTotalCount(); i++)
		phases[i] = _phases[i];
}

void Gait::setPhase(int index, uint16 phase)
{
	phases[index] = phase;
}

void Gait::fillPhases(uint16 phase)
{
	for (uint16 i = 0; i < getTotalCount(); i++)
		phases[i] = phase;
}

uint16 Gait::getPhase(uint8 index)
{
	if (phases)
		return phases[index];
	return 0;
}
