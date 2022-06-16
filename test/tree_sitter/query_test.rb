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

puts root

pattern = '(method_parameters)'
capture = '(method_parameters (_)+ @args)'
combined = "#{pattern} #{capture}"
# string = '(method_parameters (_)+ @args)'

# NOTE: It' still unclear to me what a captured string is.

describe 'pattern/capture/string' do
  it 'must return an Integer for pattern count' do
    query = TreeSitter::Query.new(ruby, pattern)
    assert_equal 1, query.pattern_count
    assert_equal 0, query.capture_count
    assert_equal 0, query.string_count
  end

  it 'must return an Integer for pattern count' do
    query = TreeSitter::Query.new(ruby, capture)
    assert_equal 1, query.pattern_count
    assert_equal 1, query.capture_count
    assert_equal 0, query.string_count
  end

  it 'must return an Integer for combined patterns' do
    query = TreeSitter::Query.new(ruby, combined)
    assert_equal 2, query.pattern_count
    assert_equal 1, query.capture_count
    assert_equal 0, query.string_count
  end

  it 'must return an Integer for pattern start byte' do
    query = TreeSitter::Query.new(ruby, combined)
    assert_equal 0, query.start_byte_for_pattern(0)
    assert_equal pattern.bytesize + 1, query.start_byte_for_pattern(1)
  end
end