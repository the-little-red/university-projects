require 'test_helper'

class AddressTest < ActiveSupport::TestCase
  # test "the truth" do
  #   assert true
  # end

  test "address must have fields" do
	address = Address.new
	assert_not address.save
  end

end
