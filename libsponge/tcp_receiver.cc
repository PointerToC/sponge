#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    TCPHeader tcp_header = seg.header();
    Buffer pay_load = seg.payload();
    WrappingInt32 seqno = tcp_header.seqno;

    if (tcp_header.syn == true) {
        this->isn = tcp_header.seqno + 1ul;
        seqno = seqno + 1ul;
        this->syn_signal = true;
        this->fin_signal = false;
        this->cnt_recv = 0;
    }
    if (tcp_header.fin == true) {
        this->fin_signal = true;
    }

    if (this->syn_signal == true) {
        this->_reassembler.push_substring(pay_load.copy(), seqno - this->isn, tcp_header.fin);
        this->cnt_recv += pay_load.size();
        if (this->cnt_recv == this->_reassembler.stream_out().bytes_written() && this->fin_signal == true) {
            this->_reassembler.stream_out().end_input();
            this->isn = this->isn + 1ul;
        }
    }
    this->ack_num = this->isn + this->_reassembler.stream_out().bytes_written();
    return;
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    if (this->syn_signal == true) {
        return this->ack_num;
    }
    return nullopt;
}

size_t TCPReceiver::window_size() const { return this->_capacity - this->_reassembler.stream_out().buffer_size(); }
