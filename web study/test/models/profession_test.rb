require 'test_helper'

class ProfessionTest < ActiveSupport::TestCase
  # test "the truth" do
  #   assert true
  # end

  test "profession must have fields" do 
	p = Profession.new
	assert_not p.save
  end
end
