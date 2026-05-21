#include "network.h"


Network::Network(size_t layers, size_t inputs, const size_t neyrones[], float (*f)(float), float (*g)(float)) : 
		layers(layers), inputs(inputs), neyrones(new size_t[layers]), f(f), g(g), data(new float **[layers]) {
	if (layers == 0 or inputs == 0 or neyrones == nullptr or f == nullptr or g == nullptr) {
		throw "error";
	}

	for (size_t i = 0; i < layers; i++) {
		if (neyrones[i] == 0) {
			delete [] this->neyrones;
			throw "error";
		}
		this->neyrones[i] = neyrones[i];
	}

	for (size_t i = 0; i < layers; i++) {
		data[i] = new float *[neyrones[i]];
		for (size_t j = 0; j < neyrones[i]; j++) {
			data[i][j] = new float[inputs+1];
		}
		//data[i][j] = new float[inputs+1];
		inputs = neyrones[i];
	}
}

Network::Network(size_t layers, size_t inputs, const size_t neyrones[], float (*f)(float), float (*g)(float), float def) : Network(layers, inputs, neyrones, f, g) {
	size_t i, j, k;
	for (i = 0; i < layers; i++) {
		for (j = 0; j < neyrones[i]; j++) {
			for (k = 0; k < inputs; k++) {
				data[i][j][k] = def;
			}
			data[i][j][k] = def;
		}
		inputs = neyrones[i];
	}
}

Network::Network(size_t layers, size_t inputs, const size_t neyrones[], float (*f)(float), float (*g)(float), float min, float max) : Network(layers, inputs, neyrones, f, g) {
	size_t i, j, k;
	for(i = 0; i < layers; i++) {
		for (j = 0; j < neyrones[i]; j++) {
			for (k = 0; j < inputs; k++) {
				data[i][j][k] = (max-min) * rand() / RAND_MAX + min;
			}
			data[i][j][k] = (max-min) * rand() / RAND_MAX + min;
		}
		inputs = neyrones[i];
	}
}

Network::Network(size_t layers, size_t inputs, const std::vector<size_t> &neyrones, float (*f)(float), float (*g)(float)) {
	if (neyrones.size() != layers) 
		throw "error";
	Network(layers, inputs, neyrones.data(), f, g);
}

Network::Network(size_t layers, size_t inputs, const std::vector<size_t> &neyrones, float (*f)(float), float (*g)(float), float def) {
	if (neyrones.size() != layers) 
		throw "error";
	Network(layers, inputs, neyrones.data(), f, g);
}

Network::Network(size_t layers, size_t inputs, const std::vector<size_t> &neyrones, float (*f)(float), float (*g)(float), float min, float max) {
	if (neyrones.size() != layers) 
		throw "error";
	Network(layers, inputs, neyrones.data(), f, g);
}

Network::Network(size_t inputs, const std::vector<size_t> &neyrones, float(*f)(float), float (*g)(float)) {
	Network(neyrones.size(), inputs, neyrones.data(), f, g);
}

Network::Network(size_t inputs, const std::vector<size_t> &neyrones, float(*f)(float), float (*g)(float), float def) {
	Network(neyrones.size(), inputs, neyrones.data(), f, g, def);
}

Network::Network(size_t inputs, const std::vector<size_t> &neyrones, float(*f)(float), float (*g)(float), float min, float max) {
	Network(neyrones.size(), inputs, neyrones.data(), f, g, min, max);
}

Network::Network(const char filename[]) {
	Network(std::string(filename));
}

Network::Network(const std::string &filename) {
	size_t i, j;
	std::ifstream file(filename, std::ios::binary);
	if (not file) 
		throw "error: could not open file";
	file.read((char *) &layers, sizeof(layers));
	file.read((char *) &inputs, sizeof(inputs));
	neyrones = new size_t[layers];
	file.read((char *) neyrones, layers * sizeof(size_t));
	data = new float **[layers];
	size_t inp = inputs;
	for (i = 0; i < layers; i++) {
		data[i] = new float *[neyrones[i]];
		for (j = 0; j < inp; j++) {
			data[i][j] = new float[inp];
			file.read((char *) data[i][j], (inp+1) * sizeof(float));
			if (not file) 
				throw "error: error while reading file";
		}
		inp = neyrones[i];
	}
}

Network::Network(const Network &other) {
	this->layers = other.layers;
	this->inputs = other.inputs;
	this->neyrones = new size_t[layers];
	for (size_t i = 0; i < layers; i++) {
		this->neyrones[i] = other.neyrones[i];
	}

	size_t inputs = this->inputs;
		
	data = new float **[layers];

	size_t i, j, k;
		
	for (i = 0; i < layers; i++) {
		data[i] = new float *[neyrones[i]];
		for (j = 0; j < neyrones[i]; j++) {
			data[i][j] = new float[inputs];
			for (k = 0; k < inputs; k++) {
				data[i][j][k] = other.data[i][j][k];
			}
			data[i][j][k] = other.data[i][j][k];
		}
		inputs = neyrones[i];
	}
}
/*
	Network &operator =(const Network &other) {
		this->~Network();
		this->Network(other);
		return *this;
	}
*/
void Network::save(const char filename[]) const {
	save(std::string(filename));
}

void Network::save(const std::string &filename) const {
	std::ofstream file(filename, std::ios::binary);
	file.write((const char *) &layers, sizeof(layers));
	file.write((const char *) &inputs, sizeof(inputs));
	size_t i, j;
	size_t inp = inputs;
	for (i = 0; i < layers; i++) {
		for (j = 0; j < inp; j++) {
			file.write((const char *) data[i][j], (inp+1) * sizeof(float));
		}
		inp = neyrones[i];
	}
}

float *Network::compute(const float input[]) const {
	size_t inputs = this->inputs;
	float s, *in = new float[inputs], *out;
	size_t i, j, k;

	for (j = 0; j < inputs; j++) {
		in[i] = input[i];
	}

	for (i = 0; i < layers; i++) {
		out = new float[neyrones[i]];
		for (j = 0; j < neyrones[i]; j++) {
			s = 0.0;
			for (k = 0; k < inputs; k++) {
				s += in[k] * data[i][j][k];
			}
			s += data[i][j][k];
			out[j] = f(s);
		}
		delete [] in;
		in = out;
		inputs = neyrones[i];
	}

	return out;
}

float *Network::compute(const std::vector<float> &input) const {
	if (input.size() != inputs) 
		throw "error: invalid size for std::vector<float> input";
	return compute(input.data());
}

void Network::compute(const float input[], float output[]) const {
	float *out = compute(input);

	for (size_t j = 0; j < neyrones[layers-1]; j++) {
		output[j] = out[j];
	}

	delete [] out;
}

void Network::compute(const std::vector<float> &input, std::vector<float> &output) const {
	if (input.size() != inputs or output.size() != neyrones[layers-1]) 
		throw "error: invalud size for std::vector<float> input or output";
	compute(input.data(), output.data());
}

union Network::Error Network::_teach(const float input[], const float output[], float n, float l, bool in_err, bool out_err) {
	size_t inputs = this->inputs;
	size_t layers = this->layers;
	float s, error = 0.0, r = 0;
	const float t = n*l;
	float **values = new float *[layers];
	float *in = new float[inputs], *out;
	size_t i, j, k;

	for (j = 0; j < inputs; j++) {
		in[j] = input[j];
	}

	if (in_err) layers--;

	for (i = 0; i < layers; i++) {
		values[i] = in;
		out = new float[neyrones[i]];
		for (j = 0; j < neyrones[i]; j++) {
			s = 0.0;
			for (k = 0; k < inputs; k++) {
				s += in[k] * data[i][j][k];
			}
			s += data[i][j][k];
			out[j] = f(s);
		}
		in = out;
		inputs = neyrones[i];
	}

	layers = this->layers;

	if (l and not out_err) {
		inputs = this->inputs;
		for (i = 0; i < layers; i++) {
			for (j = 0; j < neyrones[i]; j++) {
				for (k = 0; k < inputs; k++) {
					r += data[i][j][k] * data[i][j][k];
				}
			}
			inputs = neyrones[i];
		}
		error += l * r;
	}

	if (in_err) {
		out = new float[layers-1];
		for (j = 0; j < neyrones[layers-1]; j++) {
			out[j] = output[j];
			error += out[j] * out[j];
		}
		delete [] in;
	} else {
		for (j = 0; j < neyrones[layers-1]; j++) {
			s = output[j] - out[j];
			out[j] = s * g(out[j]);
			error += s*s;
		}
	}
	
	for (i = layers-1; ; i--) {
		if (i == 0) {
			inputs = this->inputs;
		} else {
			inputs = neyrones[i-1];
		}
		in = new float[inputs];

		for (k = 0; k < inputs; k++) {
			s = 0;
			for (j = 0; j < neyrones[i]; j++) {
				s += out[j] * data[i][j][k];
			}
			in[k] = s * g(values[i][k]);
		}
		for (j = 0; j < neyrones[i]; j++) {
			s = n * out[j];
			for (k = 0; k < inputs; k++) {
				data[i][j][k] += s * values[i][k] - t * data[i][j][k];
			}
			data[i][j][k] += s;
		}
		delete [] out;
		out = in;
		if (i == 0) break;
	}

	for (i = 0; i < layers-1; i++) {
		delete [] values[i];
	}
	if (not in_err) delete [] values[i];
	delete [] values;

	union Error result;
	if (out_err) {
		result.errors = in;
	} else {
		result.error = error / 2;
	}
	return result;
}

union Network::Error Network::_error(const float input[], const float output[], float l, bool in_err, bool out_err) const {
	size_t i, j, k;
	float error = 0, *errors, *out;

	if (not out_err) {
		if (in_err) {
			for (j = 0; j < neyrones[layers-1]; j++) {
				error += output[j] * output[j];
			}
			return error / 2;
		} else {
			out = compute(input);
			for (j = 0; j < neyrones[layers-1]; j++) {
				error += (output[j] - out[j])*(output[j] - out[j]);
			}
			delete [] out;
			return error / 2;
		}
	} else {
	}

float Network::teach(const float input[], const float output[], float n, float l) {
	return _teach(input, output, n, l, false, false).error;
}

float Network::error(const float input[], const float output[], float l) const {
	float *out = compute(input);
	float error = 0.0, r = 0.0;
	size_t inputs = this->inputs;

	for (size_t j = 0; j < neyrones[layers-1]; j++) {
		error += (output[j] - out[j])*(output[j] - out[j]);
	}

	if (l) {
		for (size_t i = 0; i < layers; i++) {
			for (size_t j = 0; j < neyrones[i]; j++) {
				for (size_t k = 0; k < inputs; k++) {
					r += data[i][j][k] * data[i][j][k];
				}
			}
			inputs = neyrones[i];
		}
		error += l * r;
	}

	delete [] out;

	return error / 2;
}

float Network::error(const std::vector<float> &input, const std::vector<float> &output, float l) const {
	if (input.size() != inputs or output.size() != neyrones[layers-1]) 
		throw "error: invalid size for input or poutput";
	return error(input.data(), output.data());
}
float Network::teach(const std::vector<float> &input, const std::vector<float> &output, float n, float l) {
	if (input.size() != inputs or output.size() != neyrones[layers-1]) 
		throw "error: invalid size for input or output";
	return teach(input.data(), output.data(), n, l);
}

float *Network::input_error(const float input[], const float output[]) const {
	size_t i, j, k;
	size_t inputs = this->inputs;
	float s, *input_errors = new float[inputs];
	float **values = new float *[layers], **errors = new float *[layers];
	float *in = new float[inputs], *out;

	for (j = 0; j < inputs; j++) {
		in[j] = input[j];
	}

	for (i = 0; i < layers; i++) {
		values[i] = in;
		errors[i] = new float[neyrones[i]];
		out = new float[neyrones[i]];
		for (j = 0; j < neyrones[i]; j++) {
			s = 0.0;
			for (k = 0; k < inputs; k++) {
				s += in[k] * data[i][j][k];
			}
			s += data[i][j][k];
			out[j] = f(s);
		}
		in = out;
		inputs = neyrones[i];
	}

	for (j = 0; j < neyrones[layers-1]; j++) {
		errors[layers-1][j] = (output[j] - out[j]) * g(out[j]);
	}
	for (i = layers-2; true; i--) {
		for (j = 0; j < neyrones[i]; j++) {
			s = 0.0;
			for (k = 0; k < neyrones[i+1]; k++) {
				s += errors[i+1][k] * data[i+1][k][j];
			}
			errors[i][j] = s * g(values[i+1][j]);
		}
		if (i == 0) break;
	}
	for (j = 0; j < this->inputs; j++) {
		s = 0.0;
		for (k = 0; k < neyrones[0]; k++) {
			s += errors[0][k] * data[0][k][j];
		}
		input_errors[j] = s * g(values[0][j]);
	}

	for (i = 0; i < layers; i++) {
		delete [] values[i];
		delete [] errors[i];
	}
	delete [] values;
	delete [] errors;
	delete [] out;

	return input_errors;
}

float *Network::input_error(const std::vector<float> &input, const std::vector<float> &output) const {
	if (input.size() != inputs or output.size() != neyrones[layers-1]) {
		throw "error: invalid vector size";
	}
	return input_error(input.data(), output.data());
}

void Network::input_error(const float input[], const float output[], float copy[]) const {
	float *out = input_error(input, output);
	for (size_t i = 0; i < inputs; i++) {
		copy[i] = out[i];
	}
	delete [] out;
}

void Network::input_error(const std::vector<float> &input, const std::vector<float> &output, std::vector<float> &copy) const {
	if (input.size() != inputs or output.size() != neyrones[layers-1] or copy.size() != inputs) {
		throw "error: invalid vector size";
	}
	input_error(input.data(), output.data(), copy.data());
}

float *Network::teach_error(const float input[], const float output[], float n, float l) {
	return _teach(input, output, n, l, false, true).errors;
}

float *Network::teach_error(const std::vector<float> &input, const std::vector<float> &output, float n, float l) {
	if (input.size() != inputs or output.size() != neyrones[layers-1]) {
		throw "error: invalid vector size";
	}
	return teach_error(input.data(), output.data(), n, l);
}

void Network::teach_error(const float input[], const float output[], float copy[], float n, float l) {
	float *out = teach_error(input, output, n, l);
	for (size_t i = 0; i < inputs; i++) {
		copy[i] = out[i];
	}
	delete [] out;
}

void Network::teach_error(const std::vector<float> &input, const std::vector<float> &output, std::vector<float> &copy, float n, float l) {
	if (input.size() != inputs or output.size() != neyrones[layers-1] or copy.size() != inputs) {
		throw "error: invalid vector size";
	}
	teach_error(input.data(), output.data(), copy.data(), n, l);
}

float Network::error_teach(const float input[], const float output[], float n, float l) {
	return _teach(input, output, n, l, true, false).error;
}

float Network::error_teach(const std::vector<float> &input, const std::vector<float> &output, float n, float l) {
	if (input.size() != inputs or output.size() != neyrones[layers-1]) {
		throw "error: invalid vector size\n";
	}
	return error_teach(input.data(), output.data(), n, l);
}

float *Network::error_teach_error(const float input[], const float output[], float n, float l) {
	return _teach(input, output, n, l, true, true).errors;
}

float *Network::error_teach_error(const std::vector<float> &input, const std::vector<float> &output, float n, float l) {
	if (input.size() != inputs or output.size() != neyrones[layers-1]) {
		throw "error: invalid vector size\n";
	}
	return error_teach_error(input.data(), output.data(), n, l);
}

void Network::error_teach_error(const float input[], const float output[], float copy[], float n, float l) {
	float *out = error_teach_error(input, output, n, l);
	for (size_t i = 0; i < neyrones[layers-1]; i++) {
		copy[i] = out[i];
	}
	delete [] out;
}

void Network::error_teach_error(const std::vector<float> &input, const std::vector<float> &output, std::vector<float> &copy, float n, float l) {
	if (input.size() != inputs or output.size() != neyrones[layers-1] or copy.size() != inputs) {
		throw "error: invalid vector size\n";
	}
	error_teach_error(input.data(), input.data(), copy.data(), n, l);
}

/*
void selfteach(float input[], float n) {
	float **values = new float *[layers+1], *in = new float[inputs], *out;
	float s;
	size_t inputs = this->inputs;
	size_t i, j, k;

	for (size_t j = 0; j < inputs; j++) {
		in[j] = input[j];
	}

	for (i = 0; i < layers; i++) {
		values[i] = in;
		out = new float[neyrones[i]];
		for (j = 0; j < neyrones[i]; j++) {
			s = 0.0;
			for (k; k < inputs; k++) {
				s += in[k] * data[i][j][k];
			}
			s += data[i][j][k];
			out[j] = f(s);
		}
		in = out;
		inputs = neyrones[i];
	}
	values[i] = out;

	inputs = this->inputs;

	for (i = 0; i < layers; i++) {
		for (j = 0; j < neyrones[i]; j++) {
			s = n * values[i+1][j];
			for (k = 0; k < inputs; k++) {
				data[i][j][k] += s * values[i][k];
			}
			data[i][j][k] += s;
		}
		inputs = neyrones[i];
	}

	for (i = 0; i < layers; i++) {
		delete [] values[i];
	}
	delete [] values;
	delete [] out;
}
*/
/*
	void learn1(float input[], float output[], float nextput[], float r, float g, float n) {
		float out[neyrones[layers-1]];

		float m = nextput[0];
		for (size_t j = 1; j < neyrones[layers-1]; j++) {
			if (m < nextput[j]) {
				m = nextput[j];
			}
		}

		for (size_t j = 0; j < neyrones[layers-1]; j++) {
			out[j] = output[j] * (r + g*m);
		}

		teach(input, out, n);
	}

	void learn2(float input[], float output[], float r, float n) {
		float out[neyrones[layers-1]];

		for (size_t j = 0; j < neyrones[layers-1]; j++) {
			out[j] = output[j] * r;
		}

		teach(input, out, n);
	}
*/
/*
	float &operator [](size_t i, size_t j, size_t k) {
		return &data[i][j][k];
	}

	float operator [](size_t i, size_t j, size_t k) const {
		return data[i][j][k];
	}
*/
/*
	bool operator ==(Network other) const {
		if (layers != other.layers or inputs != other.inputs) {
			return false;
		}

		for (size_t i = 0; i < layers; i++) {
			if (neyrones[i] != other.neyrones[i]) {
				return false;
			}
		}

		size_t inputs = this->inputs;

		for (size_t i = 0; i < layers; i++) {
			for (size_t j = 0; j < neyrones[i]; j++) {
				for (size_t k = 0; k <= inputs; k++) {
					if (data[i][j][k] != other.data[i][j][k]) {
						return false;
					}
					if (k == inputs) break;
				}
			}
			inputs = neyrones[i];
		}

		return true;
	}
*/

const size_t &Network::get_layers() const {
	return layers;
}

const size_t &Network::get_inputs() const {
	return inputs;
}

const size_t * const &Network::get_neyrones() const {
	return neyrones;
}

void Network::get_neyrones(size_t output[]) const {
	for (size_t i = 0; i < layers; i++) {
		output[i] = neyrones[i];
	}
}

void Network::get_neyrones(std::vector<size_t> output) const {
	if (output.size() != layers) 
		throw "error: invalid size for neyrones";
	get_neyrones(output.data());
}

float * const * const * const &Network::get_data() {
	return data;
}

const float * const * const * const &Network::get_data() const {
	return data;
}

float &Network::get_weight(size_t i, size_t j, size_t k) {
	return data[i][j][k];
}

const float &Network::get_weight(size_t i, size_t j, size_t k) const {
	return data[i][j][k];
}

Network::~Network() {
	for (size_t i = 0; i < layers; i++) {
		for (size_t j = 0; j < neyrones[i]; j++) {
			delete [] data[i][j];
		}
		delete [] data[i];
	}
	delete [] data;
	delete [] neyrones;
}
