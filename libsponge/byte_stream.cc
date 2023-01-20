#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) : total_cap(capacity) { DUMMY_CODE(capacity); }

size_t ByteStream::write(const string &data) {
    size_t data_len = data.size();
    size_t remain_cap = this->remaining_capacity();
    size_t data_index = 0;
    while (remain_cap > 0 && data_index < data_len) {
        this->buffer.push_back(data[data_index]);
        --remain_cap;
        ++data_index;
    }
    this->num_byte_write += data_index;
    return data_index;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    std::string res;
    size_t cnt = 0;
    for (auto iter = this->buffer.cbegin(); cnt < len && iter != this->buffer.cend(); ++cnt, ++iter) {
        res.push_back(*iter);
    }
    return res;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    size_t cnt = len;
    while (cnt > 0 && !this->buffer.empty()) {
        this->buffer.pop_front();
        --cnt;
    }
    if (this->buffer_empty() && this->input_ended() == true) {
        this->eof_signal = true;
    }
    this->num_byte_read += len;
    return;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    std::string res;
    size_t cnt = 0;
    for (auto iter = this->buffer.cbegin(); iter != this->buffer.cend() && cnt < len; ++iter, ++cnt) {
        res.push_back(*iter);
    }
    this->pop_output(cnt);
    return res;
}

void ByteStream::end_input() {
    this->intput_end_signal = true;
    if (this->num_byte_read == this->num_byte_write) {
        this->eof_signal = true;
    }
}

bool ByteStream::input_ended() const { return this->intput_end_signal; }

size_t ByteStream::buffer_size() const { return this->buffer.size(); }

bool ByteStream::buffer_empty() const { return this->buffer.empty(); }

bool ByteStream::eof() const { return this->eof_signal; }

size_t ByteStream::bytes_written() const { return this->num_byte_write; }

size_t ByteStream::bytes_read() const { return this->num_byte_read; }

size_t ByteStream::remaining_capacity() const { return this->total_cap - this->buffer.size(); }
