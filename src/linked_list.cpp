#include "linked_list.hpp"

#include <cassert>    // assert
#include <stdexcept>  // out_of_range

#include "private/internal.hpp"  // это не тот приват, о котором вы могли подумать

namespace itis {

    void LinkedList::Add(Element e) {
        if (head_ == nullptr) {
            head_ = new Node(e, nullptr);
        } else {
            Node *n = head_;
            for (int i = 0; i < size_ - 1; ++i) {
                n = n->next;
            }
            Node *newNode = new Node(e, nullptr);
            n->next = newNode;
            tail_ = newNode;
        }
        size_++;
    }

    void LinkedList::Insert(int index, Element e) {
        internal::check_out_of_range(index, 0, size_ + 1);
        if(size_ == 0 && index != 0) return;
        if(index == 0){
            Node *n = new Node(e, head_);
            head_ = n;
        }else if(index == size_){
            Node *n = new Node(e, nullptr);
            tail_->next = n;
            tail_ = n;
        }else{
            Node *n = new Node(e, find_node(index));
            find_node(index-1)->next = n;
        }
        size_++;
    }

    void LinkedList::Set(int index, Element e) {
        internal::check_out_of_range(index, 0, size_);
        find_node(index)->data = e;
    }

    Element LinkedList::Remove(int index) {
        internal::check_out_of_range(index, 0, size_);
        Node *n = head_;
        if(index == 0){
            head_ = head_->next;
        }else{
            n = find_node(index);
            find_node(index-1)->next = n->next;
        }
        size_--;
        return n->data;
    }

    void LinkedList::Clear() {
        Node *n = head_;
        Node *n2;
        for (int i = 0; i < size_ - 1; ++i) {
            n2 = n;
            n = n->next;
            delete n2;
        }
        size_ = 0;
        head_ = nullptr;
        tail_ = nullptr;
    }

    Element LinkedList::Get(int index) const {
        internal::check_out_of_range(index, 0, size_);
        return find_node(index)->data;
    }

    int LinkedList::IndexOf(Element e) const {
        Node *n = head_;
        for(int i = 0; i < size_; i++){
            if(n->data == e) return i;
            n = n->next;
        }
        return -1;
    }

    Node *LinkedList::find_node(int index) const {
        assert(index >= 0 && index < size_);
        Node *n = head_;
        for (int i = 0; i < index; ++i) {
            n = n->next;
        }
        return n;
    }

// РЕАЛИЗОВАНО

    LinkedList::~LinkedList() {
        Clear();
    }

    bool LinkedList::Contains(Element e) const {
        // если индекс не найден, значит и элемента нет
        return IndexOf(e) == kNotFoundElementIndex;
    }

    int LinkedList::GetSize() const {
        return size_;
    }

    bool LinkedList::IsEmpty() const {
        return size_ == 0;
    }

    Element LinkedList::tail() const {
        // вместо выброса ошибки в случае nullptr, римским парламентов было решено возвращать "специальное" значение
        return tail_ ? tail_->data : Element::UNINITIALIZED;
    }

    Element LinkedList::head() const {
        return head_ ? head_->data : Element::UNINITIALIZED;
    }

// === RESTRICTED AREA: необходимо для тестирования ===

    LinkedList::LinkedList(const std::vector<Element> &elements) {
        assert(!elements.empty());

        size_ = elements.size();
        head_ = new Node(elements[0], nullptr);

        auto current_node = head_;

        for (int index = 1; index < static_cast<int>(elements.size()); index++) {
            current_node->next = new Node(elements[index], nullptr);
            current_node = current_node->next;
        }
        tail_ = current_node;
    }

    std::ostream &operator<<(std::ostream &os, const LinkedList &list) {
        if (list.head_ != nullptr && list.tail_ != nullptr) {
            os << "{ ";
            for (auto current_node = list.head_; current_node != list.tail_; current_node = current_node->next) {
                os << internal::elem_to_str(current_node->data) << ", ";
            }
            os << internal::elem_to_str(list.tail_->data) << " }";
        } else {
            os << "{ nullptr }";
        }
        return os;
    }

    bool operator==(const LinkedList &list, const std::vector<Element> &elements) {
        if (list.size_ != static_cast<int>(elements.size())) return false;
        Node *current_node = list.head_;

        for (const auto e : elements) {
            if (current_node == nullptr) return false;
            if (current_node->data != e) return false;
            current_node = current_node->next;
        }
        return true;
    }

}  // namespace itis