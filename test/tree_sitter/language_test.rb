# frozen_string_literal: true

require_relative '../test_helper.rb'

ruby = lang('ruby')
parser = TreeSitter::Parser.new
parser.language = ruby

program = <<~RUBY
  def mul(a, b)
    res = a* b
    puts res.inspect
    return res
  end
RUBY

tree = parser.parse_string(nil, program)
root = tree.root_node

# NOTE: one should be weary of testing with data structures that are owned by
# parsers.   They are not reliable and we should expect them to break when these
# parsers evolve.

describe 'language' do
  it 'must return symbol count' do
    assert ruby.symbol_count.positive?
  end

  it 'must return symbol name' do
    assert_equal 'end', ruby.symbol_name(0)
  end

  it 'must return symbol id for string name' do
    assert ruby.symbol_for_name(root.type, root.named?).positive?
  end

  it 'must return field count' do
    assert ruby.field_count.positive?
  end

  it 'must return field name for id' do
    assert_equal 'alias', ruby.field_name_for_id(1)
  end
end
