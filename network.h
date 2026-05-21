#pragma once
#ifndef _NETWORK_H_
#define _NETWORK_H_
#include <string>
#include <vector>
#include <fstream>


class Network {
	protected:
		union Error {
			float error, *errors;
		};

		size_t layers, inputs, *neyrones;
		float ***data;

		union Error _teach(const float [], const float [], float, float, bool, bool);
		union Error _error(const float [], const float [], float, float, bool, bool);

	public:
		float (*f)(float);
		float (*g)(float);

		Network() = delete;
		Network(size_t, size_t, const size_t [], float (*)(float), float (*)(float));
		Network(size_t, size_t, const size_t [], float (*)(float), float (*)(float), float);
		Network(size_t, size_t, const size_t [], float (*)(float), float (*)(float), float, float);
		Network(size_t, size_t, const std::vector<size_t> &, float (*)(float), float (*)(float));
		Network(size_t, size_t, const std::vector<size_t> &, float (*)(float), float (*)(float), float);
		Network(size_t, size_t, const std::vector<size_t> &, float (*)(float), float (*)(float), float, float);
		Network(size_t, const std::vector<size_t> &, float (*)(float), float (*)(float));
		Network(size_t, const std::vector<size_t> &, float (*)(float), float (*)(float), float);
		Network(size_t, const std::vector<size_t> &, float (*)(float), float (*)(float), float, float);
		Network(const char []);
		Network(const std::string &);
		Network(const Network &);

		void save(const char []) const;
		void save(const std::string &) const;

		float *compute(const float []) const;
		float *compute(const std::vector<float> &) const;
		void compute(const float [], float []) const;
		void compute(const std::vector<float> &, std::vector<float> &) const;

		float error(const float [], const float [], float = 0) const;
		float error(const std::vector<float> &, const std::vector<float> &, float = 0) const;

		float teach(const float [], const float [], float, float = 0); // + функция потер (w += dw - lambda*2*w); error += sum(w**2 for w in data)
		float teach(const std::vector<float> &, const std::vector<float> &, float, float = 0);

		float *input_error(const float [], const float []) const;
		float *input_error(const std::vector<float> &, const std::vector<float> &) const;
		void input_error(const float [], const float [], float []) const;
		void input_error(const std::vector<float> &, const std::vector<float> &, std::vector<float> &) const;

		float *teach_error(const float [], const float [], float, float = 0);
		float *teach_error(const std::vector<float> &, const std::vector<float> &, float, float = 0);
		void teach_error(const float [], const float [], float [], float, float = 0);
		void teach_error(const std::vector<float> &, const std::vector<float> &, std::vector<float> &, float, float = 0);

		float output_error(const float [], const float [], float, float = 0) const;
		float output_error(const std::vector<float> &, const std::vector<float> &, float, float = 0) const;

		float error_teach(const float [], const float [], float, float = 0);
		float error_teach(const std::vector<float> &, const std::vector<float> &, float, float = 0);

		float *error_error(const float [], const float [], float, float = 0) const;
		float *error_error(const std::vector<float> &, const std::vector<float> &, float, float = 0), const;
		void error_error(const float [], const float [], float [], float, float = 0) const;
		void error_error(const std::vector<float> &, const std::vector<float> &, std::vector<float> &, float, float = 0) const;

		float *error_teach_error(const float [], const float [], float, float = 0);
		float *error_teach_error(const std::vector<float> &, const std::vector<float> &, float, float = 0);
		void error_teach_error(const float [], const float [], float [], float, float = 0);
		void error_teach_error(const std::vector<float> &, const std::vector<float> &, std::vector<float> &, float, float = 0);

		const size_t &get_layers() const;
		const size_t &get_inputs() const;
		const size_t * const &get_neyrones() const;
		void get_neyrones(size_t []) const;
		void get_neyrones(std::vector<size_t>) const;
		float * const * const * const &get_data();
		const float * const * const * const &get_data() const;
		float &get_weight(size_t i, size_t j, size_t k);
		const float &get_weight(size_t i, size_t j, size_t k) const;

		~Network();
};

#endif
