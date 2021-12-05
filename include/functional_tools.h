//
// Created by ykanya on 2021/12/04.
//
#pragma once
#ifndef SABRMETRICS_CXX_FUNCTIONAL_TOOLS_H
#define SABRMETRICS_CXX_FUNCTIONAL_TOOLS_H

#include<iterator>

template<class Range>
class containerizer {
    const Range &range;

public:
    explicit containerizer(const Range &r) noexcept: range(r) {}

    template<class To>
    explicit operator To() const // これのおかげで戻り値によるオーバーロードもどきができる
    {
        using std::begin; using std::end;
        return To(begin(range), end(range));
    }
};

template<class Range>
static inline containerizer<Range> containerize(Range &&range) {
    return containerizer<Range>(std::forward<Range>(range));
}

struct containerized_tag {
};
static constexpr containerized_tag containerized; // 今回はただのタグ

template<class Range>
static inline containerizer<Range> operator|(
        Range &&range, containerized_tag) { return containerize(std::forward<Range>(range)); }

#endif //SABRMETRICS_CXX_FUNCTIONAL_TOOLS_H
