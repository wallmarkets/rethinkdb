#include "containers/buffer_group.hpp"

void buffer_group_copy_data(const buffer_group_t *dest, const const_buffer_group_t *source) {

    int bytes = source->get_size();
    rassert(bytes == (int)dest->get_size());

    /* Copy data between source and dest; we have to always copy the minimum of the sizes of the
    next chunk that each one has */
    int source_buf = 0, source_off = 0, dest_buf = 0, dest_off = 0;
    while (bytes > 0) {
        while (source->get_buffer(source_buf).size == source_off) {
            source_buf++;
            source_off = 0;
        }
        while (dest->get_buffer(dest_buf).size == dest_off) {
            dest_buf++;
            dest_off = 0;
        }
        int chunk = std::min(
            source->get_buffer(source_buf).size - source_off,
            dest->get_buffer(dest_buf).size - dest_off);
        memcpy(
            reinterpret_cast<char *>(dest->get_buffer(dest_buf).data) + dest_off,
            reinterpret_cast<const char *>(source->get_buffer(source_buf).data) + source_off,
            chunk);
        source_off += chunk;
        dest_off += chunk;
        bytes -= chunk;
    }

    /* Make sure we reached the end of both source and dest */
    rassert(
        (source_buf == (int)source->num_buffers()     && source_off == 0) ||
        (source_buf == (int)source->num_buffers() - 1 && source_off == source->get_buffer(source_buf).size));
    rassert(
        (dest_buf == (int)dest->num_buffers()     && dest_off == 0) ||
        (dest_buf == (int)dest->num_buffers() - 1 && dest_off == dest->get_buffer(dest_buf).size));
}
