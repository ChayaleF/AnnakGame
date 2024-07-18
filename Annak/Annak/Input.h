#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Command.h"
class Input
{
    std::string m_infile;
    std::vector<std::string> m_lines;
    std::vector<std::string>::iterator m_at_line;
    void start_lines();
    std::string next_line();
public:
    Input(const std::string& _infile, const std::string& _outfile)
        : m_infile(_infile), outfile(_outfile)
    {
        m_at_line = m_lines.end();
        world.reset(new Command(Command::WORLD));
    }
    void parse_and_store();
    std::shared_ptr<Command> world;
    std::vector< std::shared_ptr<Command> > start;
    std::vector< std::shared_ptr<Command> > steps;
    std::string outfile;
    std::vector<std::string> asserts;
    static std::shared_ptr<Command> parse_command(std::string line);
};