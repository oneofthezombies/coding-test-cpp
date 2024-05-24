/**
 * Queue impl begin
 */

#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>

template <typename T> class Queue {
public:
  auto push(T &&value) noexcept -> void {
    std::lock_guard<std::mutex> lock(m);
    q.push(std::move(value));
    cv.notify_one();
  }

  auto push(const T &value) noexcept -> void {
    std::lock_guard<std::mutex> lock(m);
    q.push(value);
    cv.notify_one();
  }

  auto pop() noexcept -> T {
    std::unique_lock<std::mutex> lock(m);
    cv.wait(lock, [this] { return !q.empty(); });
    T value = std::move(q.front());
    q.pop();
    return std::move(value);
  }

  auto try_pop() noexcept -> std::optional<T> {
    std::lock_guard<std::mutex> lock(m);
    if (q.empty()) {
      return std::nullopt;
    }
    T value = std::move(q.front());
    q.pop();
    return std::move(value);
  }

  auto is_empty() noexcept -> bool {
    std::lock_guard<std::mutex> lock(m);
    return q.empty();
  }

private:
  std::deque<T> q;
  std::mutex m;
  std::condition_variable cv;
};

/**
 * Queue impl end
 */

#include <iostream>
using namespace std;

auto main(int argc, char **argv) -> int {
  ios_base::sync_with_stdio(false);
  cout << "Hello, World!\n";
  return 0;
}
