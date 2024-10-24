#include "Writer.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

#include "MaeBlock.hpp"

using namespace std;

namespace schrodinger
{
namespace mae
{

Writer::Writer(std::shared_ptr<ostream> stream) : m_out(std::move(stream))
{
    write_opening_block();
}

Writer::Writer(const std::string& fname)
{
    const auto ios_mode = std::ios_base::out | std::ios_base::binary;

    auto* file_stream = new ofstream(fname, ios_mode);
    m_out.reset(static_cast<ostream*>(file_stream));

    if (m_out->fail()) {
        std::stringstream ss;
        ss << "Failed to open file \"" << fname << "\" for writing operation.";
        throw std::runtime_error(ss.str());
    }

    write_opening_block();
}

void Writer::write(const std::shared_ptr<Block>& block)
{
    block->write(*m_out);
}

void Writer::write_opening_block()
{
    shared_ptr<Block> b = make_shared<Block>("");
    b->setStringProperty("s_m_m2io_version", "2.0.0");
    write(b);
}

} // namespace mae
} // namespace schrodinger
