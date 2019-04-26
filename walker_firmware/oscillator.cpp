#include "oscillator.h"

#ifdef IS_QT
	#include "arduinopolyfill.h"
	#include <math.h>
#endif

/*
------------------------------------------------------------------------------
	 ^ X (amplitude)
	 |
	,|, amp  _....._
	 |    ,="       "=.
	 |  ,"             ".                           ,"
	 |,"        ,        ".,          ,          ,,"
"""".*""""""""""|""""""""""|."""""""""|""""""""".|"""">
  ." |<--------------------->".               ."       Y (time)
,"   |        period           "._         _,"
	,|,                           "-.....-"
	 |  -amp
	 |
------------------------------------------------------------------------------
 * Amplitude	= max from 0 and min from 0, ex amp = 5, oscillate between 5 and -5
 * Phase		= time (Y) offset, in degrees. Ex amp = 5 & phase = 90, iteration 0 = 5
 * period		= time between two wave, longer period strech the wave
 * offset		= Additional fixed offset for the amplitude (X). Ex amp = 5 and offset = 10, oscillate between 15 and 5
 */

Oscillator::Oscillator()
{
	_period = 2000;
	_amplitude = 50;
	_phase = 0;
	_offset = 0;
	_stop = true;
	_ref_time = millis();
	_delta_time = 0;
	_flag = 0;
	_period_modifier = 0;
}

float Oscillator::refresh()
{
	if (!_stop)
	{
		_delta_time = (millis() - _ref_time) % getModifiedPeriod();
		_output = (float)_amplitude * sin(time_to_radians(_delta_time) + degrees_to_radians(_phase)) + _offset;
		handleFlag();
	}

	return _output;
}

void Oscillator::handleFlag()
{
	if (_flag == OSCILLATE_NORMAL)
		return;

	if (_output < 0 && _flag & OSCILLATE_POSITIVE)
		_output = -_output;
	if (_output > 0 && _flag & OSCILLATE_NEGATIVE)
		_output = -_output;
	if (_output > 0 && _flag & OSCILLATE_INVERT_POSITIVE)
		_output = -_output;
	if (_output < 0 && _flag & OSCILLATE_INVERT_NEGATIVE)
		_output = -_output;
	if (_output < 0 && _flag & OSCILLATE_ONLY_POSITIVE)
		_output = 0;
	if (_output > 0 && _flag & OSCILLATE_ONLY_NEGATIVE)
		_output = 0;
}

void Oscillator::reset()
{
	_ref_time = millis();
}

void Oscillator::start()
{
	reset();
	_stop = false;
}

void Oscillator::start(unsigned long ref_time)
{
	_ref_time = ref_time;
	_stop = false;
}

void Oscillator::stop()
{
	_stop = true;
}

void Oscillator::setPeriod(uint16 period)
{
	_period = period;
}

void Oscillator::setAmplitude(int8 amplitude)
{
	_amplitude = amplitude;
}

void Oscillator::setPhase(uint16 phase)
{
	_phase = phase;
}

void Oscillator::setOffset(int8 offset)
{
	_offset = offset;
}

void Oscillator::setTime(unsigned long ref)
{
	_ref_time = ref;
}

void Oscillator::setFlag(unsigned int flag)
{
	_flag = flag;
}

void Oscillator::setPeriodModifier(float *modifier)
{
	_period_modifier = modifier;
}

uint16 Oscillator::getModifiedPeriod()
{
	if (!_period_modifier)
		return _period;
	return (uint16)(_period * *_period_modifier);
}

float Oscillator::getOutput()
{
	return _output;
}

unsigned long Oscillator::getTime()
{
	return _ref_time;
}

float Oscillator::getPhaseProgress()
{
	return ((float)_delta_time / getModifiedPeriod()) * 360;
}

float Oscillator::time_to_radians(double time)
{
	return (time * DOUBLE_PI) / getModifiedPeriod();
}

float Oscillator::degrees_to_radians(float degrees)
{
	return (degrees * DOUBLE_PI) / 360;
}

float Oscillator::degrees_to_time(float degrees)
{
	return (degrees * getModifiedPeriod()) / 360;
}
