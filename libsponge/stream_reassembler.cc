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
    size_t strlen = data.size();

    if (eof == true) {
        this->_output.end_input();
    }

    if (strlen + this->_output.buffer_size() + this->_unorder_num > this->_capacity) {
        return;
    }

    this->_window_len = this->_capacity - this->_output.buffer_size();

    if (this->_memo.count(index) || index + strlen > this->_cursor + this->_window_len) {
        return;
    }

    if (this->_cursor == index) {
        string assem_str = data;
        size_t cnt = 0;
        size_t next_idx = index + strlen;
        while (this->_memo.count(next_idx)) {
            size_t l = this->_memo[next_idx].size();
            assem_str += this->_memo[next_idx];
            this->_memo.erase(next_idx);
            cnt += l;
            next_idx += l;
        }
        this->_cursor = next_idx;
        this->_unorder_num -= cnt;
        this->_output.write(assem_str);
        this->_window_len = this->_capacity - this->_output.buffer_size();
    } else if (this->_cursor < index) {
        this->_memo.insert(std::make_pair(index, data));
        this->_unorder_num += data.size();
    } else if (this->_cursor > index) {
        size_t gap = this->_cursor - index;

    }
}

// bool StreamReassembler::_is_overlap(const size_t &index, const size_t &strlen) const {
//     auto iter = this->_memo.lower_bound(index);
//     if (iter != this->_memo.end()) {
//         size_t start = iter->first;
//         size_t end = iter->first + iter->second.size();
//         if ((start <= index && index < end) || (start <= index + strlen && index + strlen < end) ||
//             (index < start && end <= index + strlen)) {
//             return true;
//         }
//     }
//     if (!this->_memo.empty() && iter != this->_memo.begin()) {
//         --iter;
//         size_t start = iter->first;
//         size_t end = iter->first + iter->second.size();
//         if (start < index && index < start + end) {
//             return true;
//         }
//     }
//     return false;
// }

size_t StreamReassembler::unassembled_bytes() const { return this->_unorder_num; }

bool StreamReassembler::empty() const { return this->unassembled_bytes() == 0 ? true : false; }
