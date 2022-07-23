template<typename T>
VertexBuffer<T>::~VertexBuffer(){
}

template<typename T>
void VertexBuffer<T>::init(NearLib* lib, bool dynamic, const T* data, size_t count){
  initRaw(lib, dynamic, data, sizeof(T) * count);
  vertexCount = static_cast<unsigned int>(count);
  stride = sizeof(T);
}

template<typename T>
void VertexBuffer<T>::set(const T* data, size_t count){
  setRaw(data, sizeof(T) * count);
}
