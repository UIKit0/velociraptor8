namespace uitask {

// Call Initialize() at program startup and Destroy() at the end
void Initialize();
void Destroy();

// call only from the same thread as Initialize() and Destroy()
void DrainQueue();

// Can be called from any thread. Queues the task to be executed
// as soon as possible on ui thread.
void Post(const std::function<void()>&);
}
