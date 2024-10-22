#include "Reader.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

namespace schrodinger
{
namespace mae
{

Reader::Reader(FILE* file, size_t buffer_size)
{
    m_mae_parser.reset(new MaeParser(file, buffer_size));
}

Reader::Reader(const std::shared_ptr<std::istream>& stream, size_t buffer_size)
{
    m_mae_parser.reset(new MaeParser(stream, buffer_size));
}

Reader::Reader(const std::string& fname, size_t buffer_size)
{
    const auto ios_mode = std::ios_base::in | std::ios_base::binary;

    std::shared_ptr<std::istream> stream;
    auto* file_stream = new std::ifstream(fname, ios_mode);
    stream.reset(static_cast<std::istream*>(file_stream));

    if (stream->fail()) {
        std::stringstream ss;
        ss << "Failed to open file \"" << fname << "\" for reading operation.";
        throw std::runtime_error(ss.str());
    }

    m_mae_parser.reset(new MaeParser(stream, buffer_size));
}

Reader::Reader(std::shared_ptr<MaeParser> mae_parser)
    : m_mae_parser(std::move(mae_parser))
{
}

std::shared_ptr<Block> Reader::next(const std::string& outer_block_name)
{
    std::shared_ptr<Block> block;
    do {
        m_mae_parser->whitespace();
        block = m_mae_parser->outerBlock();
    } while (block != nullptr && block->getName() != outer_block_name);
    return block;
}
} // namespace mae
} // namespace schrodinger
