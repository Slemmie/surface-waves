#pragma once

#include <iostream>

class Fps {
	
public:
	
	Fps() :
	m_fps(0),
	m_time_prv(0.0f),
	m_frame_cnt(0),
	m_enable_tracing(false),
	m_trace_interval(1.0f),
	m_time_prv_trace(0.0f),
	m_enable_trace_low_fps(false),
	m_alert_when_below(20.0f)
	{ }
	
	inline void frame(float time_now) {
		static bool first_second = true;
		m_frame_cnt++;
		if (time_now - m_time_prv >= 1.0f) {
			m_fps = m_frame_cnt;
			m_time_prv = time_now;
			m_frame_cnt = 0;
			if (m_fps < m_alert_when_below && m_enable_trace_low_fps && !first_second) {
				std::cerr << "fps: " << fps() << std::endl;
			}
			first_second = false;
		}
		if (!m_enable_tracing) {
			return;
		}
		if (time_now - m_time_prv_trace >= m_trace_interval) {
			std::cerr << "fps: " << fps() << std::endl;
			m_time_prv_trace = time_now;
		}
	}
	
	// interval in seconds
	inline void set_trace_interval(const float interval) {
		m_trace_interval = interval;
	}
	
	// interval in seconds, trace only appears if previous (every 1s) fps was below alert_when_below
	inline void set_trace_low_fps(const bool enable, const float alert_when_below = 20.0f) {
		m_enable_trace_low_fps = enable;
		if (enable) {
			m_alert_when_below = alert_when_below;
		}
	}
	
	inline void enable_tracing(const bool enable) {
		m_enable_tracing = enable;
		m_time_prv_trace = 0.0f;
	}
	
	inline float fps() const {
		return m_fps;
	}
	
private:
	
	unsigned int m_fps;
	
	float m_time_prv;
	unsigned int m_frame_cnt;
	
	bool m_enable_tracing;
	float m_trace_interval;
	
	float m_time_prv_trace;
	
	bool m_enable_trace_low_fps;
	float m_alert_when_below;
	
};
