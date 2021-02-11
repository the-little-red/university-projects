require 'test_helper'

class UserTest < ActiveSupport::TestCase
  # test "the truth" do
  #   assert true
  # end

  test "user must have username" do
	user = User.new
	assert_not user.save
  end

end
