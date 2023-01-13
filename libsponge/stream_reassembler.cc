#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity) {
    this->_window_len = this->_capacity;
}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    DUMMY_CODE(data, index, eof);
    this->_window_len = this->_capacity - this->_output.buffer_size();
    size_t idx = index;
    size_t left = 0;
    size_t len = data.size();
    // if data is already written into output,return
    if (index + data.size() < this->_cursor) {
        return;
    }
    // data is overlaped with the cursor
    if (index < this->_cursor && this->_cursor < index + data.size()) {
        left = this->_cursor - index;
        idx = this->_cursor;
        len -= left;
    }
    // length of data is execced the 'this->_window_len'
    if (index + data.size() > this->_cursor + this->_window_len) {
        len -= ((index + data.size()) - (this->_cursor + this->_window_len));
    }
    string str = data.substr(left, len);
    this->insert_str(idx, str);

    // erase overlapping
    std::string assem_str;
    std::map<size_t, std::string>::iterator iter = this->_memo.begin();
    while (iter->first == this->_cursor && iter != this->_memo.end()) {
        assem_str += iter->second;
        this->_cursor += iter->second.size();
        iter = next(iter);
        this->_memo.erase(prev(iter));
    }
    // count unreassemable data bytes
    if (assem_str.empty() == false) {
        this->_output.write(assem_str);
    }
    this->_unorder_num = 0;
    for (auto a = this->_memo.begin(); a != this->_memo.end(); ++a) {
        this->_unorder_num += a->second.size();
    }
    this->_window_len = this->_capacity - this->_output.buffer_size();
    // eof procedure
    if (eof == true) {
        this->_eof_index = index + data.size();
        this->_eof_signal = true;
    }
    if (this->_eof_signal == true && this->_eof_index == this->_cursor) {
        this->_output.end_input();
    }
    return;
}

void StreamReassembler::insert_str(const size_t &index, const std::string &str) {
    if (this->_memo.count(index)) {
        this->_memo[index] = this->_memo[index].size() > str.size() ? this->_memo[index] : str;
    } else {
        this->_memo.insert(std::make_pair(index, str));
    }
    map<size_t, std::string>::iterator iter = this->_memo.find(index);
    auto next_iter = next(iter);
    while (next_iter != this->_memo.end() && iter->first + iter->second.size() >= next_iter->first) {
        size_t diff = iter->first + iter->second.size() - next_iter->first;
        if (diff < next_iter->second.size()) {
            iter->second += next_iter->second.substr(diff);
        }
        this->_memo.erase(next_iter);
        next_iter = next(iter);
    }
    if (iter != this->_memo.begin()) {
        auto prev_iter = prev(iter);
        while (iter != this->_memo.end() && prev_iter->first + prev_iter->second.size() >= iter->first) {
            size_t diff = prev_iter->first + prev_iter->second.size() - iter->first;
            if (diff < iter->second.size()) {
                prev_iter->second += iter->second.substr(diff);
            }
            this->_memo.erase(iter);
            iter = next(prev_iter);
        }
    }
    return;
}

size_t StreamReassembler::unassembled_bytes() const { return this->_unorder_num; }

bool StreamReassembler::empty() const { return this->unassembled_bytes() == 0 ? true : false; }
